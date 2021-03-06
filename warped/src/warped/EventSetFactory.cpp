// See copyright notice in file Copyright in the root directory of this archive.

#include "EventSetFactory.h"
#include "EventSet.h"
#include "SplayTree.h"
#include "SingleLinkedList.h"
#include "SimulationConfiguration.h"
#include "TimeWarpSimulationManager.h"
#include <utils/Debug.h>

using std::cerr;
using std::endl;

EventSetFactory::EventSetFactory(){}

EventSetFactory::~EventSetFactory(){}

Configurable *
EventSetFactory::allocate( SimulationConfiguration &configuration,
			   Configurable *parent ) const {
  SimulationManager *mySimulationManager = 
    dynamic_cast<SimulationManager *>(parent);
  ASSERT( mySimulationManager != 0 );
 
  EventSet *retval = 0;
  // the following cases are possible:

  // (1) only a event set choice is specified, in which case we
  // proceed assuming that only a centralized event set is desired.

  // (2) following the event set choice, the word "all" is specified
  // meaning that a separate event set is to be maintained for each
  // simulation object.

  if( configuration.eventListTypeIs( SplayTree::getType() ) ){
    retval = new SplayTree();
    utils::debug << "Configured a SplayTree as the event set" << endl;
  }
  else if( configuration.eventListTypeIs( SingleLinkedList::getType() ) ){
    retval = new SingleLinkedList();
    utils::debug << "Configured a SingleLinkedList as the event set" << endl;
  }
  else {
    mySimulationManager->shutdown( "Invalid EventList configuration value - \"" + 
				   configuration.getEventListType() + "\"" );
  }

  return retval;
}

const EventSetFactory *
EventSetFactory::instance(){
  static EventSetFactory *singleton = new EventSetFactory();
  return singleton;
}


