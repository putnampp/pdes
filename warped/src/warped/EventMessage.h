#ifndef EVENT_MESSAGE_H
#define EVENT_MESSAGE_H

// See copyright notice in file Copyright in the root directory of this archive.

/**
   This is the class defining the termination token that gets passed
   around.  Essentially, it has a color and a terminator.  

   The color can be WHITE, which means it's on the first cycle,
   RED, which means it's on it's second cycle, or BLACK.
*/

class SerializedInstance;

#include "EventMessageBase.h"

class Event;

/**
   This class represents the token that gets passed between
   TokenPassingTerminationManager instances.

   @see TokenPassingTerminationManager
*/

class EventMessage : public EventMessageBase {
public:
  EventMessage(	unsigned int source,
		unsigned int dest,
		const Event *initEvent,
		const string &initGVTInfo ) :
    EventMessageBase( source, dest, initEvent, initGVTInfo ){}
  
  static Serializable *deserialize( SerializedInstance *data );
  static const string &getEventMessageType();

  const string &getDataType() const {
    return getEventMessageType();
  }

  static void registerDeserializer();

private:
};

#endif
