/****************************************************************************************
* File: util_assert.h
*
* Description: Provides assert macros that act similar to printf() which allows to use
*              variable arguments with a specified format.  Or if no message is needed
*              then will automatically log the condition that fails.
*
* Notes:  - The 'debug_' asserts are compiled out unless ENABLE_DEBUG_ASSERTS is non-zero.
*         - A newline is automatically appended to all assert messages.
*
*
* Usage examples:
*
*   assert_msg(1 == 2, ASSERT_CONTINUE); // BAD. If you don't have a message then don't use _msg postfix.
*   assert_msg(1 == 2, ASSERT_CONTINUE, "Some message to be logged"); // GOOD.
*
*   // The ASSERT_CONTINUE means just log assert message and resume normal code execution.
*
*   // Can also use formatting just like printf() uses.
*   assert_msg(1 == 2, ASSERT_CONTINUE, "Received %d packets expected %d.", 98, 100);
*
*   // You don't always need to include an explicit message. Here the string "1 == 2" will be logged.
*   assert(1 == 2, ASSERT_RESTART);
*
*   // The ASSERT_RESTART means you would like the system to try to restart itself and recover.
*
*   // Sometimes you always want an assert to fail. The following are exactly the same.
*   assert_always_msg(ASSERT_STOP, "Some message to be logged");
*   assert_msg(false, ASSERT_STOP, "Some message to be logged");
*
*   // The ASSERT_STOP means to go into an infinite while loop.  This should only occur
*   // during system setup.
*
* Created: 12/30/2013, by Kyle McGahee
****************************************************************************************/

#ifndef UTIL_ASSERT_INCLUDED
#define UTIL_ASSERT_INCLUDED

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

enum
{
	ASSERT_CONTINUE,  // Log failed assert message and then keep going like nothing happened.
	ASSERT_RESTART,   // Do a full system restart into RC mode.
	ASSERT_STOP,      // Log assert and then go into infinite loop.
};

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                       SETTINGS
*--------------------------------------------------------------------------------------*/

#define INCLUDE_META_INFO       1  // Include file name and line number for every assert.
#define ENABLE_DEBUG_ASSERTS    1  // Turn on asserts that are explicitly labeled 'debug'.

/*---------------------------------------------------------------------------------------
*                                        MACROS
*--------------------------------------------------------------------------------------*/

// Include file name and line number for every assert.
#if INCLUDE_META_INFO
#define META_INFO_ARGS __FILE__,__LINE__
#else
#define META_INFO_ARGS "",(-1)
#endif

// Will be automatically placed before any debug assert message.
#define DEBUG_MARK_STRING "D: "

// See top of file for usage examples.  The ellipses (...) allow a macro to accept a variable
// number of arguments after the preceding required ones.  In the macro signiture the variable
// arguments replace the __VA_ARGS__ token specifier.  The meta info args refers to the file
// and line number that the assert is on.  If an assert fails the 'util_assert_failed()'
// function is called which will log the assert and take the specified action.  If no
// message is provided then use the 'assert' macro which will take the condition passed in
// and reformat it to a string which will be logged like a normal message (which is pretty cool).
#define assert_msg(condition, action, ...) (!(condition) ? util_assert_failed((action), META_INFO_ARGS, __VA_ARGS__) : (void)0)
#define assert_always_msg(action, ...) assert_msg(false, (action), __VA_ARGS__)
#define assert(condition, action) assert_msg((condition), (action), #condition)

#if ENABLE_DEBUG_ASSERTS

#define debug_assert_msg(condition, action, ...) (!(condition) ? util_assert_failed((action), META_INFO_ARGS, DEBUG_MARK_STRING __VA_ARGS__) : (void)0)
#define debug_assert_always_msg(action, ...) debug_assert_msg(false, (action), __VA_ARGS__)
#define debug_assert(condition, action) debug_assert_msg((condition), (action), #condition)

#else

#define debug_assert_msg(condition, action, ...)
#define debug_assert_always(action)
#define debug_assert(condition, action)

#endif

/*---------------------------------------------------------------------------------------
*                                      PROTOTYPES
*--------------------------------------------------------------------------------------*/

// Do not call directly.  Instead use macros provided above that will call this funtion
// if an assert condition fails.
void util_assert_failed
	(
		int action,             // What to do after logging.  Such as restarting system.
		char const * file_name, // File name that assert failed in.
		int line_number,        // Line number that assert failed on.
		char * format,          // Message to log.
		...                     // Variable arguments. (just like printf() uses)
	);

#endif // UTIL_ASSERT_INCLUDED
