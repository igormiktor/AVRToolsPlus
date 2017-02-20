EventManager                   {#EventManager}
============


Using an event-driven design is a common way to code AVR projects that interact
with the environment around them.  With EventManager you register functions
that "listen" for particular events.  When things happen you "post" events
to EventManager.  You then periodically call an EventManager function which
processes events by dispatching them so that he appropriate listeners are called.

EventManger is interrupt safe, so that you can post events from interrupt
handlers.  The corresponding listeners will be called from outside the interrupt
handler in your main thread of execution when you call the EventManager function
to process events.

In keeping with the limited resources of an AVR system, EventManager is
light-weight.  There is no dynamic memory allocation.  Event
queuing is very fast, so you can be comfortable queuing events from interrupt
handlers.  To keep the footprint minimal, the event queue and
the listener list (also known as the dispatch table) are both small (although
you can make them bigger if needed).

EventManager supports both high priority and low priority events, and will
process high priority events ahead of any low priority events.

EventManager is implemented as a set of functions contained with the namespace
EventManager.  Implementing EventManager this was instead of as a class removes
the need to create and share an EventManager object across your code.  However,
it does limit your code to a single EventManager and therefore a single set of
event queues.


# Usage #                   {#EventManagerUsage}

Using EventManager is straightforward.  You include EventManager.h in files that access
the EventManager and link against the file `EventManager.cpp`.  You register listener functions using EventManager::addListener(), you post events using EventManager::queueEvent(), and you process events by calling EventManager::processEvent().

The following sections explain this in more detail.



# Events #                   {#EventManagerEvents}

EventManager events consist of an event code and an event parameter.  Both of these are integer values.  The event code identifies the type of event.  For
convenience, EventManager.h provides a set of constants you can use to identify events:

~~~{.cpp}
    EventManager::kEventKeyPress
    EventManager::kEventKeyRelease
    EventManager::kEventChar
    EventManager::kEventTime
    EventManager::kEventTimer0
    EventManager::kEventTimer1
    EventManager::kEventTimer2
    EventManager::kEventTimer3
    EventManager::kEventAnalog0
    EventManager::kEventAnalog1
    EventManager::kEventAnalog2
    EventManager::kEventAnalog3
    EventManager::kEventAnalog4
    EventManager::kEventAnalog5
    EventManager::kEventMenu0
    EventManager::kEventMenu1
    EventManager::kEventMenu2
    EventManager::kEventMenu3
    EventManager::kEventMenu4
    EventManager::kEventMenu5
    EventManager::kEventMenu6
    EventManager::kEventMenu7
    EventManager::kEventMenu8
    EventManager::kEventMenu9
    EventManager::kEventSerial
    EventManager::kEventPaint
    EventManager::kEventUser0
    EventManager::kEventUser1
    EventManager::kEventUser2
    EventManager::kEventUser3
    EventManager::kEventUser4
    EventManager::kEventUser5
    EventManager::kEventUser6
    EventManager::kEventUser7
    EventManager::kEventUser8
    EventManager::kEventUser9
~~~

These are purely for your convenience; EventManager only uses the numerical
value to match events to listeners, so you are free to use any event codes you
wish.

The event parameter is also whatever you want it to be.  EventManager simply
passes the event parameter to every listener function that is associated with
that event code.  For example, for a key press event the event parameter could
be the corresponding key code.  For an analog event it could be the value read
from that analog pin or a pin number.

You post events using the EventManager::queueEvent() function, like this:

~~~{.cpp}
    EventManager::queueEvent( EventManager::kEventUser0, 1234 );
~~~

The EventManager::queueEvent() function is lightweight and interrupt safe, so
you can call it from inside an interrupt handler.

By default the event queue holds 8 events, but you can make the event queue any
size you want by defining the macro EVENTMANAGER_EVENT_QUEUE_SIZE to whatever
value you desire
(see [Increasing Event Queue Size](##EventManagerIncreaseQueueListSize)
below).


# Listeners #                   {#EventManagerListeners}

Listeners are functions of type

~~~{.cpp}
    typedef void ( *EventListener )( int eventCode, int eventParam );
~~~

You add listeners using the EventManager::addListener() function, like this:

~~~{.cpp}
    void myListener( int eventCode, int eventParam )
    {
        // Do something with the event
    }

    void setup()
    {
        EVentManager::addListener( EventManager::kEventUser0, myListener );

        // Do more set up
    }
~~~

\note Do \e not add listeners from within an interrupt routine.

By default the list of listeners holds 8 listeners, but you can make the list
any size you want by defining the macro EVENTMANAGER_DISPATCH_TABLE_SIZE to
whatever value you desire
(see [Increasing Listener List Size](##EventManagerIncreaseListenerListSize)
below).



# Processing Events #           {#EventManagerProcessEvents}

To process events in the event queue and dispatch them to listeners you
call EventManager::processEvent() from a top-level event handling
loop:

~~~{.cpp}
    void loop()
    {
        // Do stuff that might generate events

        // Process events
        EventManager::processEvent();
    }
~~~

This call processes one event from the event queue every time it is called.
Usually EventManager::processEvent() is called once in the inside of some main
event loop so that one event is handled every time through the loop. This is
usually more than adequate to keep up with incoming events.  Events are normally
processed in a first-in, first-out fashion (but see the section on
[Event Priority](#EventManagerEventPriority) below).


# Advanced Details #             {#EventManagerAdvancedTopics}

The remaining sections provide additional details about Event Manager that allowed more tailor and flexible usage.


## Event Priority ##              {#EventManagerEventPriority}

EventManager recognizes high and low priority events.  You can specify the
priority when you queue the event.  By default, events are considered low
priority.  You indicate an event is high priority by passing an additional
constant to EventManager::queueEvent(), like so:

~~~{.cpp}
    EventManager::queueEvent( EventManager::kEventUser0, 1234, EventManager::kHighPriority );
~~~

The difference between high and low priority events is that
EventManager::processEvent() will process a high priority event ahead of any low
priority events.  In effect, high priority events jump to the front of the queue
(multiple high priority events are processed first-in, first-out, but all of
them are processed before any low priority events).

\note If high priority events are queued faster than low priority events,
EventManager may never get to processing any of the low priority events.  So use
high priority events judiciously.


## Interrupt Safety ##              {#EventManagerInterruptSafety}

EventManager is interrupt safe, so that you can queue events both from within
interrupt handlers and also from normal functions without having to worry about
queue corruption.  This safety is achieved by globally disabling interrupts
while certain small snippets of code are executing.


## Processing All Events ##         {#EventManagerProcessAllEvents}

Normally calling EventManager::processEvent() once every time through a
top-level event processing loop is more than adequate to service incoming
events.  However, there may be times when you want to process all the events in
the queue.  For this purpose you can call EventManager::processAllEvents(). Note
that if you call this function at the same time that a series of events are
being rapidly added to the queue asynchronously (for example, via interrupt
handlers), the EventManager::processAllEvents() function might not return until
the series of additions to the event queue stops.


## Increasing Event Queue Size ##   {#EventManagerIncreaseEventQueueSize}

Define the macro `EVENTMANAGER_EVENT_QUEUE_SIZE` to whatever size you need at
compile time by passing it to the compiler on the command line using something
like: `-DEVENTMANAGER_EVENT_QUEUE_SIZE=32`

The event queue requires `4 * sizeof(int) = 8` bytes for each unit of size.
There is a factor of 4 (instead of 2) because internally EventManager
maintains two separate queues: a high-priority queue and a low-priority queue.


## Increasing Listener List Size ##     {#EventManagerIncreaseListenerListSize}

Define the macro `EVENTMANAGER_LISTENER_LIST_SIZE` to whatever size you need at
compile time by passing it to the compiler on the command line using something
like: : `-DEVENTMANAGER_LISTENER_LIST_SIZE=32`

The listener list requires `sizeof(*f()) + sizeof(int) + sizeof(boolean) = 5`
bytes for each unit of size.


## Additional Features ##            {#EventManagerAdditionalFeatures}

There are various functions for managing the listeners:

- You can remove listeners using EventManager::removeListener()
- Disable and enable specific listeners using EventManager::enableListener()
- Set a default listener using EventManager::setDefaultListener();  the default
listener will handle any events not handled by other listeners
- Manipulate the default listener just like any other listener using
EventManager::removeDefaultListener() and EventManager::enableDefaultListener()
- Check if the listener list is full using EventManager::isListenerListFull()
- Check if the listener list is empty using EventManager::isListenerListEmpty()


There are various class functions that provide information about the event
queue:

- Check if the event queue is full using EventManager::isEventQueueFull()
- Check if the event queue is empty using EventManager::isEventQueueEmpty()
- See how many events are in the event queue using EventManager::getNumEventsInQueue()

For details on these functions you should review EventManager.h documentation.


## Credits ##                       {#EventManagerCredits}

EventManager was inspired by the Arduino Event System by mromani@ottotecnica.com
of OTTOTECNICA Italy.


