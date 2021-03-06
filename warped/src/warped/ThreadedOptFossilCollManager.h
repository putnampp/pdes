// See copyright notice in file Copyright in the root directory of this archive.

#ifndef THREADEDOPTFOSSILCOLLMANAGER_H_
#define THREADEDOPTFOSSILCOLLMANAGER_H_

#include "warped.h"
#include "CommunicatingEntity.h"
#include "Configurable.h"
#include "StateSetObject.h"
#include "OutputManager.h"
#include <set>
#include <fstream>
#include <map>
#include "ThreadedTimeWarpSimulationManager.h"

using namespace std;

class ThreadedTimeWarpSimulationManager;
class TimeWarpSimulationManager;
class SimulationObject;
class VTime;
class CommunicationManager;
class Event;
class NegativeEvent;
class RestoreCkptMessage;
class State;

/** The Optimistic Fossil Collection Manager class.

 This class implements optimistic fossil collection.

 */
class ThreadedOptFossilCollManager: virtual public CommunicatingEntity,
		virtual public Configurable {
public:
	/**@name Public Class Methods of OptFossilCollManager. */
	//@{

	/** Constructor.

	 @param sim Handle to the simulation manager.
	 @param checkPeriod The amount to increment the checkpoint by.
	 @param minimumSamples The minimum rollback samples taken before calculating the active history length.
	 @param maximumSamples The maximum rollback samples.
	 @param defaultLength The initial active history length of all objects.
	 @param risk The risk safety factor. Higher values have less risk of catastrophic rollback.
	 */
			ThreadedOptFossilCollManager(ThreadedTimeWarpSimulationManager *sim,
					int checkPeriod, int minimumSamples, int maximumSamples,
					int defaultLen, double risk);

	// Destructor.
	virtual ~ThreadedOptFossilCollManager();

	// Configure.
	void configure(SimulationConfiguration &configuration);

	// Register OptFossilCollManager specific message types with the comm. manager
	virtual void registerWithCommunicationManager();

	// The method the communication manager will call to deliver messages
	virtual void receiveKernelMessage(KernelMessage *msg);

	/** Return the last time of garbage collection for the object.

	 @param id The simulation object id of the object.
	 @return int The time of garbage collection for the object.
	 */
	inline int getLastCollectTime(unsigned int id) {
		return lastCollectTimes[id];
	}

	// Returns the Rollback Time during restoration when one thread initiates recovery
	unsigned int getRestoreRollbackTime() {
		return restoreRollbackTime;
	}

	/** Saves the simulation state when the time to be examined is greater
	 than the next checkpoint time.

	 @param checkTime The time to be examined.
	 @param objId The simulation object id of the object to be examined.
	 */
	virtual void checkpoint(const VTime &checkTime, const ObjectID &objId,
			const unsigned int &threadId);

	/** Save the checkpoint at the beginning of the simulation.
	 */
	virtual void makeInitialCheckpoint();

	/** Restores a given checkpoint state.

	 @param restoredTime The time of the checkpoint to restore.
	 */
	virtual void restoreCheckpoint(unsigned int restoredTime);

	/** Updates the next checkpoint time if necessary. If the time is less
	 than the last checkpoint time, the last checkpoint time and the next
	 checkpoint time are updated accordingly.

	 @param objId The simulation object id of the object to examine.
	 @param time The time to examine.
	 */
	virtual void updateCheckpointTime(unsigned int objId, int time);

	/** Sets the flag inRecovery and allows the manager to start the recovery process

	 @param objectId The simulation object id of the object that caused the rollback.
	 */
	virtual void setRecovery(unsigned int objectId, unsigned int rollbackTime);

	/** Starts the recovery from a catastrophic rollback. Stops all processing
	 on the simulation manager and sends a restore message.
	 */
	virtual void startRecovery();

	/** Calculates the checkpoint to restore, purges all the queues, resets
	 *	the collect times and restores from the last checkpoint
	 */
	virtual void purgeQueuesAndRecover();

	/** Performs fossil collect for a single object based on the object's
	 active history length.

	 @param object The object to perform fossil collection for.
	 @param currentTime Used to determine a proper fossil collection time.
	 */
	virtual void fossilCollect(SimulationObject *object,
			const VTime &currentTime, const unsigned int &threadId);

	/** Samples rollback lengths and calculates the active history length.

	 @param obj The object that is rolling back.
	 @param time The rollback time to be sampled.
	 */
	virtual void sampleRollback(SimulationObject *obj, const VTime &time) = 0;

	/** Used to handle a special case rollback after recovery has occurred.
	 It is only called when a fault occurs. It checks to see if a recovery
	 just occurred and restores the proper state if necessary.

	 @param obj The simulation object that is rolling back.
	 @return bool True if this an actual fault. False otherwise.
	 */
	virtual bool checkFault(SimulationObject *obj);

	virtual int getLeastCollectTime();

	/*// These are not currently used anywhere. It was a potential solution
	 // that did not work out as well as was hoped.
	 void *newEvent(size_t size);

	 void *newNegativeEvent(size_t size);

	 State *newState(SimulationObject *);

	 void deleteEvent(void *);

	 void deleteNegativeEvent(void *);

	 void deleteState(const State *, unsigned int);

	 void setStateQueue(multiset< SetObject<State> > *initStateQueue);
	 */

	//@} // End of Public Class Methods of OptFossilCollManager. */

protected:
	/**@name protected Class Methods of OptFossilCollManager. */
	//@{

	//TimeWarpSimulationManager *mySimManager;
	ThreadedTimeWarpSimulationManager *mySimManager;
	CommunicationManager *myCommManager;

	/// The active history length of the object.
	vector<int> activeHistoryLength;

	/// The time of the most recent fossil collection for an object.
	vector<int> lastCollectTimes;

	/// The period at which fossil collection is performed.
	vector<int> fossilPeriod;

	/// The time of the initial checkpoint save.
	const int firstCheckpointTime;

	/// The time of the next checkpoint save.
	vector<int> nextCheckpointTime;

	/// The time of the last checkpoint save.
	vector<int> lastCheckpointTime;

	/// The time of the last restored checkpoint. -1 if no restores have occurred.
	int lastRestoreTime;

	/// The period at which simulation state is to be saved.
	int checkpointPeriod;

	/// The risk of having a catastrophic rollback. Higher values provide
	/// a lower chance of having a rollback. Range is 0 < riskFactor < 1.
	double riskFactor;

	/// The initial active history length of all objects.
	int defaultLength;

	/// The number of rollback samples that have been taken.
	vector<int> numSamples;

	/// The minimum number of rollback samples taken before calculating
	/// the active history length.
	int minSamples;

	/// The maximum number of rollback samples taken.
	int maxSamples;

	/// The simulation manager to which restore messages are sent.
	int myPeer;

	/// True if recovering from a catastrophic rollback.
	bool recovering;

	/// The saved states for every object at checkpoint times. These
	/// are not written to a file because there is no function to serialize states.
	map<int, vector<State*>*> checkpointedStates;

	/// The path to the directory where the OFC checkpoint files are stored.
	string ckptFilePath;

	// Time to rollback to while starting a recovery
	unsigned int restoreRollbackTime;

	/*// These are not currently used anywhere. It was a potential solution
	 // that did not work out as well as was hoped.
	 // The state queue from the state manager.
	 multiset< SetObject<State> > *stateQueue;
	 vector<NegativeEvent*> availableNegEventMem;
	 vector<NegativeEvent*> inUseNegEventMem;
	 vector< vector<State*> > availableStateMem;
	 */

	//@} // End of Protected Class Methods of OptFossilCollManager.
};

#endif /* ThreadedOPTFOSSILCOLLMANAGER_H_ */
