// See copyright notice in file Copyright in the root directory of this archive.

#include "RealDVFSManager.h"
#include "TimeWarpSimulationManager.h"
#include "UsefulWorkMessage.h"
#include "CommunicationManager.h"
#include <utils/Debug.h>

using namespace std;

RealDVFSManager::RealDVFSManager(TimeWarpSimulationManager* simMgr,
                                 int measurementPeriod,
                                 int firsize,
                                 bool fixed,
                                 bool debug,
                                 OptimizationGoal og,
                                 UsefulWorkMetric uwm)

  :DVFSManagerImplementationBase(simMgr,
                                 measurementPeriod,
                                 firsize,
                                 fixed,
                                 debug,
                                 og,
                                 uwm)
{}

void
RealDVFSManager::poll() {
  if(checkMeasurementPeriod()) {
    // initiate the measurement cycle
    if(isMaster()) {
      int dest = (mySimulationManagerID + 1) % myNumSimulationManagers;
      UsefulWorkMessage* msg = new UsefulWorkMessage(mySimulationManagerID,
                                                     dest,
                                                     myNumSimulationManagers,
                                                     UsefulWorkMessage::COLLECT);
      myCommunicationManager->sendMessage(msg, dest);
    }
  }
}

void
RealDVFSManager::registerWithCommunicationManager() {
  myCommunicationManager->registerMessageType(UsefulWorkMessage::dataType(), this);
}

void
RealDVFSManager::configure(SimulationConfiguration &config) {
  // populate available frequencies and our CPU id, set userspace governor
  DVFSManagerImplementationBase::configure(config);

  // initialize the frequency index array now that we know how many
  // frequencies are available
  int maxidx = myAvailableFreqs.size() - 1;

  // build the frequency index array based on the number of P states available
  initializeFrequencyIdxs(maxidx);

  // initialize my frequency to the median frequency
  int freq = myAvailableFreqs[maxidx / 2];
  cout << "(" << mySimulationManagerID << "): bound to PE " << myCPU
       << "; initializing freq to " << freq << endl;
  setCPUFrequency(myCPU, freq);
}

void
RealDVFSManager::receiveKernelMessage(KernelMessage* kMsg) {
  UsefulWorkMessage* msg = dynamic_cast<UsefulWorkMessage*>(kMsg);
  ASSERT(msg);

  std::vector<double> dat;
  msg->getData(dat);

  // add our useful work index to the array
  fillUsefulWork(dat);
  if(isMaster()) {

    // update FIR filters with data. if we're the master, then we know
    // dat has all the current useful work indexes of all other LPs
    for(int i = 0; i < dat.size(); ++i)
      myUtilFilters[i].update(dat[i]);

    // if we're not staying at a fixed frequency, and the frequency indexes 
    // have changed, then set the new cpu frequencies
    if(!isDummy() && updateFrequencyIdxs())
      for(int i=0; i < myFrequencyIdxs.size(); i++)
        setCPUFrequency(i, myAvailableFreqs[myFrequencyIdxs[i]]);

    // write trace to csv
    if(debugPrint())
      for(int i=0; i < myFrequencyIdxs.size(); i++)
        writeCSVRow(i, 
                    myUtilFilters[i].getData(), 
                    myAvailableFreqs[myFrequencyIdxs[i]]);

  }
  else {

    // send the useful work array on to the next simulation manager
    int dest = (mySimulationManagerID + 1) % myNumSimulationManagers;
    UsefulWorkMessage* newMsg = new UsefulWorkMessage(mySimulationManagerID,
                                                        dest,
                                                        myNumSimulationManagers,
                                                        UsefulWorkMessage::COLLECT);
    newMsg->setData(dat);
    myCommunicationManager->sendMessage(newMsg, dest);
  }

  delete kMsg;
}

string
RealDVFSManager::toString() {
  return "Real DVFS, " + DVFSManagerImplementationBase::toString();
}