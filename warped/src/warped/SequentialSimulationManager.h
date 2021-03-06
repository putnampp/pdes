#ifndef SEQUENTIAL_SIMULATION_MANAGER_H
#define SEQUENTIAL_SIMULATION_MANAGER_H

// See copyright notice in file Copyright in the root directory of this archive.

#include "warped.h"
#include "SimulationManagerImplementationBase.h"
#include "SequentialSimulationStream.h"
#include "EventSet.h"
#include "StopWatch.h"

class Application;

/** The SequentialSimulationManager class.

    The SequentialSimulationManager class implements a simulation manager 
    that performs a sequential discrete-event simulation.

*/
class SequentialSimulationManager : public SimulationManagerImplementationBase{

public:

  /**@type friend class declarations */
  //@{

  /** Builder class */
  friend class SequentialConfigurationManager;

  //@} // End of friend class declarations  

  /**@name Public Class Methods of SequentialSimulationManager. */
  //@{
  
  /** Constructor.

  */
  SequentialSimulationManager( Application * initApplication );
   
  /// Destructor.
  ~SequentialSimulationManager();

  void initialize();

  /** Perform the sequential simulation.
  @param simulateUntil Time to simulate until.
  */
  void simulate ( const VTime& simulateUntil );

  void finalize();

  /** Receive an event.

  @param event Pointer to the received event.
  */
  void handleEvent( const Event *event );

  /** Get and remove event for simulation object.

  @param object Simulation object whose event to get.
  @return Pointer to the event.
  */
  const Event *getEvent(SimulationObject *object);

  /** Get a pointer to the next event for a simulation object.
      
  @param object Simulation object whose event to peek at.
  @return Pointer to the event.
  */
  const Event *peekEvent(SimulationObject *object);

  /// registers a set of simulation objects with this simulation manager.
  void registerSimulationObjects ();
   
  /** Get the current simulation time.

  @return The current simulation time.
  */
  const VTime &getSimulationTime() const { return *simulationTime; }

  /** Get object handle with string object as lookup.

  @param object String used to look up object.
  @return Handle to the object.
  */
  SimulationObject *getObjectHandle( const string &object ) const {
	typeSimMap::const_iterator it = localArrayOfSimObjPtrs->find(object);
	if (it == localArrayOfSimObjPtrs->end())
		return 0;
    return it->second;
  }

  /** Get object handle with object Id as lookup.

  @param objectID Object Id used to look up object.
  @return Handle to the object.
  */
  SimulationObject *getObjectHandle( const OBJECT_ID &objectID ){
    return localArrayOfSimObjIDs[objectID.getSimulationObjectID()];
  }

  /// get a handle to a simulation input stream
  SimulationStream *getIFStream(const string &filename,
				SimulationObject *object);

  /// get a handle to a simulation output stream
  SimulationStream *getOFStream(const string &filename,
				SimulationObject *object,
				ios::openmode mode=ios::out);

  /// get a handle to a simulation input-output stream
  SimulationStream *getIOFStream(const string &filename,
				 SimulationObject *object);

  virtual void configure( SimulationConfiguration &configuration );
   
  /**
     @see SimulationManager#contains.  This implementation always returns
     true since there can only be one SimulationManager.
  */
  bool contains( const string & ){ return true; }

  const VTime &getCommittedTime();
  const VTime &getNextEventTime();
  const VTime &getPositiveInfinity() const;
  const VTime &getZero() const;

  bool simulationComplete();

  void shutdown( const string &errorMessage );

  //@} // End of Public Class Methods of SequentialSimulationManager.

protected:
  /**@name Protected Class Methods of SequentialSimulationManager. */
  //@{
  /// Mapping between simulation object names, object pointers & id
  //typedef map<string, SimulationObject * >  typeNewSimMap;
    
  typeSimMap *createMapOfObjects();
  //@} // End of Protected Class Methods of SequentialSimulationManager.

  /**@name Protected Class Attributes of SequentialSimulationManager. */
  //@{

  /// This is my current simulation time.
  const VTime *simulationTime;
   
  /// the number of processed events
  unsigned int numberOfProcessedEvents;
   
  /// This is the handle to set of pending events
  EventSet *myEventSet;

  //@} // End of Protected Class Attributes of SequentialSimulationManager.

private:
  /**@name Private Class Attributes of SequentialSimulationManager. */
  //@{

  void setSimulationTime( const VTime &newTime ){
    delete simulationTime;
    simulationTime = newTime.clone();
  }

  ///Local stream to be associated with global stream "wout"
  SequentialSimulationStream sequentialWout;

  
  ///Local stream to be associated with global stream "werr"
  SequentialSimulationStream sequentialWerr;

  /**
     The application I am managing.
  */
  Application *myApplication;

  StopWatch initializeWatch;
  double totalSimulationTime;

  //@} // End of Protected Class Attributes of SequentialSimulationManager.
};

#endif
