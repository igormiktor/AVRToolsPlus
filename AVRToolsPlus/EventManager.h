/*
 * EventManager.h
 * An event handling system for avr microcontrollers.
 *
 * Author: igormt@alumni.caltech.edu
 * Copyright (c) 2017 Igor Mikolic-Torreira
 *
 * Inspired by and adapted from the
 * Arduino Event System library by
 * Author: mromani@ottotecnica.com
 * Copyright (c) 2010 OTTOTECNICA Italy
 *
 * This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with this library; if not,
 * write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */


/*!
 * \file
 *
 * \brief This file provides an Event Manager using  functional interface under the namespace EventManager.
 *
 * To use these functions, include EventManager.h and link against EventManager.cpp.
 *
 * The event queue and and listener list are arrays of fixed size.  The size of
 * both can be set at compile time.
 *
 * The default size of the dispatch table is 8 (event, listener) pairs;
 * you can change the default at compile time by defining the macro \c EVENTMANAGER_DISPATCH_TABLE_SIZE prior to including the
 * file EventManager.h, each time it is included.  Define it using a compiler option
 * (e.g., \c -DEVENTMANAGER_DISPATCH_TABLE_SIZE=32) to ensure it is consistently defined throughout your project.
 *
 * The default size of the event queues is 8 events (note there are two queues, one for routine priority events),
 * the other for high priority events.  You can change the default size of both of the event queues at compile time
 * by defining the macro \c EVENTMANAGER_EVENT_QUEUE_SIZE prior to including the
 * file EventManager.h, each time it is included.  Define it using a compiler option
 * (e.g., \c -DEVENTMANAGER_EVENT_QUEUE_SIZE=32) to ensure it is consistently defined throughout your project.
 *
 */



#ifndef EventManager_h
#define EventManager_h


#include <stdint.h>



// Size of the listener list.  Adjust as appropriate for your application.
// Requires a total of sizeof(*f())+sizeof(int)+sizeof(bool) bytes of RAM for each unit of size
#ifndef EVENTMANAGER_DISPATCH_TABLE_SIZE
#define EVENTMANAGER_DISPATCH_TABLE_SIZE        8
#endif

#if EVENTMANAGER_DISPATCH_TABLE_SIZE > 255
#error "EVENTMANAGER_DISPATCH_TABLE_SIZE exceeds size of a uint8_t"
#endif




// Size of the event two queues.  Adjust as appropriate for your application.
// Requires a total of 4 * sizeof(int) bytes of RAM for each unit of size
#ifndef EVENTMANAGER_EVENT_QUEUE_SIZE
#define EVENTMANAGER_EVENT_QUEUE_SIZE           8
#endif

#if EVENTMANAGER_EVENT_QUEUE_SIZE > 255
#error "EVENTMANAGER_EVENT_QUEUE_SIZE exceeds size of a uint8_t"
#endif







/*!
 * \brief This namespace bundles the Event Manager functionality.  It provides logical cohesion
 * for functions implement the Event Manager and prevents namespace collisions.
*/

namespace EventManager
{

    /*!
    * \brief This enum provides common event names, purely for user convenience.
    *
    * \hideinitializer
    */

    enum GenericEvents
    {
        kEventNone,
        kEventKeyPress,
        kEventKeyRelease,
        kEventChar,
        kEventTime,
        kEventTimer0,
        kEventTimer1,
        kEventTimer2,
        kEventTimer3,
        kEventAnalog0,
        kEventAnalog1,
        kEventAnalog2,
        kEventAnalog3,
        kEventAnalog4,
        kEventAnalog5,
        kEventMenu0,
        kEventMenu1,
        kEventMenu2,
        kEventMenu3,
        kEventMenu4,
        kEventMenu5,
        kEventMenu6,
        kEventMenu7,
        kEventMenu8,
        kEventMenu9,
        kEventSerial,
        kEventPaint,
        kEventUser0,
        kEventUser1,
        kEventUser2,
        kEventUser3,
        kEventUser4,
        kEventUser5,
        kEventUser6,
        kEventUser7,
        kEventUser8,
        kEventUser9,
    };



    /*!
    * \brief Type for an event listener (a.k.a. callback) function.
    *
    */

    typedef void ( *EventListener )( int eventCode, int eventParam );



    /*!
    * \brief EventManager recognizes two kinds of events.  By default, events are
    * are queued as low priority, but these constants can be used to explicitly
    * set the priority when queueing events.
    *
    * \note High priority events are always handled before any low priority events.
    *
    * \hideinitializer
    */

    enum EventPriority
    {
        kHighPriority,
        kLowPriority
    };



    /*!
    * \brief Add an (event, listener) pair listener to the dispatch table.
    *
    * \arg \c eventCode the event code this listener listens for.
    * \arg \c listener the listener to be called when there is an event with this eventCode.
    *
    * \returns True if (the event, listener) pair is successfully installed in the dispatch table,
    * false otherwise (e.g. the dispatch table is full).
    */

    bool addListener( int eventCode, EventListener listener );



    /*!
    * \brief Remove this (event, listener) pair from the dispatch table.
    * Other listener pairs with the same function or event code will not be affected.
    *
    * \arg \c eventCode the event code of the (event, listener) pair to be removed.
    * \arg \c listener the listener of the (event, listener) pair to be removed.
    *
    * \returns True if the (event, listener) pair is successfully removed, false otherwise.
    */

    bool removeListener( int eventCode, EventListener listener );



    /*!
    * \brief Remove all occurrances of a listener from the dispatch table, regardless of the event code.
    * returns number removed.
    *
    * This function is useful when one listener handles many different events.
    *
    * \arg \c listener the listener to be removed.
    *
    * \returns The number of entries removed from the dispatch table.
    *
    */

    int removeListener( EventListener listener );



    /*!
    * \brief Enable or disable an (event, listener) pair entry in the dispatch table.
    *
    * \arg \c eventCode the event code of the (event, listener) pair to be enabled or disabled.
    * \arg \c listener the listener of the (event, listener) pair to be enabled or disabled.
    * \arg \c enable pass true to enable the (event, listener) pair, false to disable it.
    *
    * \returns True if the (event, listener) pair was successfully enabled or disabled,
    * false if the (event, listener) pair was not found.
    */

    bool enableListener( int eventCode, EventListener listener, bool enable );



    /*!
    * \brief Obtain the the current enabled/disabled state of an (eventCode, listener) pair.
    *
    * \arg \c eventCode the event code of the (event, listener) pair.
    * \arg \c listener the listener of the (event, listener) pair.
    *
    *
    * \returns The current enabled/disabled state of the (eventCode, listener) pair.
    */

    bool isListenerEnabled( int eventCode, EventListener listener );



    /*!
    * \brief Set a default listener.  The default listener is a callback function that is called when an
    * event with no listener is processed.
    *
    * \arg \c listener the listener to be set as the default listener.
    *
    * \returns True if the default listener is successfully installed, false if \c listener is null.
    */

    bool setDefaultListener( EventListener listener );



    /*!
    * \brief Remvoes the default listener.  The default listener is a callback function that is called when an
    * event with no listener is processed.
    */

    void removeDefaultListener();



    /*!
    * \brief Enable or disable the default listener.  The default listener is a callback function that is called when an
    * event with no listener is processed.
    *
    * \arg \c enable pass true to enable the default listener, false to disable it.
    */

    void enableDefaultListener( bool enable );



    /*!
    * \brief Check if the listener list (a.k.a., dispatch table) is empty.
    *
    * \returns True if the listener list (dispatch table) is empty, false if not.
    */

    bool isListenerListEmpty();



    /*!
    * \brief Check if the listener list (a.k.a., dispatch table) is full.
    *
    * \returns True if the listener list (dispatch table) is full, false if not.
    */

    bool isListenerListFull();



    /*!
    * \brief Get the number of listeners in the dispatch table.
    *
    * \returns The number of entries in the listener list (a.k.a., dispatch table).
    */

    int numListeners();



    /*!
    * \brief Check if the event queue is empty.
    *
    * \arg \c pri the desired event queue: kLowPriority or kHighPriority.  Defaults to kLowPriority.
    *
    * \returns True if the specified event queue is empty.
    */

    bool isEventQueueEmpty( EventPriority pri = kLowPriority );



    /*!
    * \brief Check if the event queue is full.
    *
    * \arg \c pri the desired event queue: kLowPriority or kHighPriority.  Defaults to kLowPriority.
    *
    * \returns True if the specified event queue is full.
    */

    bool isEventQueueFull( EventPriority pri = kLowPriority );



    /*!
    * \brief Get the number of events in the event queue.
    *
    * \arg \c pri the desired event queue: kLowPriority or kHighPriority.  Defaults to kLowPriority.
    *
    * \returns The number of events in the specified event queue.
    */

    int getNumEventsInQueue( EventPriority pri = kLowPriority );



    /*!
    * \brief Tries to add an event into the event queue.
    *
    * \arg \c eventCode  identifies the event to be added.
    * \arg \c eventParam  an integer parameter associated with this event.
    * \arg \c pri specifies which queue gets the event: kLowPriority or kHighPriority.  Defaults to kLowPriority.
    *
    * \returns True if successful; false if the queue is full and the event cannot be added.
    */

    bool queueEvent( int eventCode, int eventParam, EventPriority pri = kLowPriority );



    /*!
    * \brief Processes one event from the event queue and
    * dispatches it to the corresponding listeners stored in the dispatch table.
    *
    * Events are taken preferentially from the high priority queue.  If the high priority queue is empty,
    * then events are taken from the low prioirty queue.
    *
    * All listeners associated with the event that are enabled will be called.  Disabled listeners are not called.
    *
    * The event processed is removed from the event queue (even if there is no listener to handle it).
    *
    * \note This function should be called regularly to keep the event queues from getting full.  This is usually
    * done by calling it inside an event processing loop.
    *
    * \returns The number of event handlers called.
    */

    int processEvent();



    /*!
    * \brief Processes \e all the events in the event queues and dispatches them to the corresponding listeners
    * stored in the dispatch table.
    *
    * Events are taken preferentially from the high priority queue.  If the high priority queue is empty,
    * then events are taken from the low prioirty queue.
    *
    * All listeners associated with the event that are enabled will be called.  Disabled listeners are not called.
    *
    * The event processed is removed from the event queue (even if there is no listener to handle it).
    *
    * \note If interrupts or other asynchronous processes are adding events as they are being processed,
    * this function might not return for a long time.  If events are added as quickly as this function
    * processes them, this function will never return.  .
    *
    * \returns The number of event handlers called.
    */

    int processAllEvents();

};




#endif
