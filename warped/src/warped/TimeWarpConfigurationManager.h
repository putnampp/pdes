#ifndef TIMEWARP_CONFIGURATION_MANAGER_H
#define TIMEWARP_CONFIGURATION_MANAGER_H

// See copyright notice in file Copyright in the root directory of this archive.

#include "warped.h"
#include "ConfigurationManager.h"

#include "TimeWarpSimulationManager.h"


// components of a basic TimeWarp system
#include "TimeWarpEventSetFactory.h"
#include "TimeWarpSimulationObjectQueueFactory.h"
#include "StateManagerFactory.h"
#include "OutputManagerFactory.h"
#include "SchedulingManagerFactory.h"
#include "CommunicationManagerFactory.h"
#include "GVTManagerFactory.h"

/** The TimeWarpConfigurationManager class.

    The TimeWarpConfigurationManager class represents the concrete
    class that is responsible for constructing and configuring a
    TimeWarp Simulation Manager and its components.
*/
class TimeWarpConfigurationManager : public ConfigurationManager {
public:
   
  /**@name Public Class Methods of TimeWarpConfigurationManager */
  //@{

  /** Default constructor 

  @param numSimMgrs The number of simulation managers that have been
  requested.
  @param arguments The list of arguments passed in.
  @param initApplication The application that we are starting with.

  */
  TimeWarpConfigurationManager( const vector<string> &arguments,
				Application *initApplication );
   
  /// Default destructor
  ~TimeWarpConfigurationManager();

  /** configure the specified simulation manager.

  @return returns a configured simulation manager
  */
  void configure( SimulationConfiguration &configuration );
  
  /**
     Gets the simulation manager for this configuration.
     
     @return The Simulation Manager;
  */
  SimulationManager *getSimulationManager();

  //@} // End of Public Class Methods of TimeWarpConfigurationManager

protected:

  /**@name Protected Class Attributes of TimeWarpConfigurationManager */
  //@{

  /// the handle to the simulation manager
  TimeWarpSimulationManager *mySimulationManager;

  const vector<string> &myCommandLineArguments;

  /// This is the handle to TimeWarp EventSet Factory
  TimeWarpEventSetFactory *myEventSetFactory;

  /// handle to the StateManager Factory
  StateManagerFactory *myStateFactory;
   
  // handle to the GVTManager Factory
  GVTManagerFactory *myGVTFactory;
   
  /// Handle to the communication manager Factory
  CommunicationManagerFactory *myCommunicationFactory;

  /// Handle to the scheduler Factory
  SchedulingManagerFactory *mySchedulingFactory;

  /// Handle to the output manager Factory
  OutputManagerFactory *myOutputFactory;

  /// Handle to the application we're associated with
  Application *myApplication;

  //@} // End of Protected Class Attributes of TimeWarpConfigurationManager

private:

};
#endif
