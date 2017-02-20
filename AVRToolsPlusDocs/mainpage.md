AVRToolsPlus:  A library of higher-level tools for the AVR ATmega328 and ATmega2560 Microcontrollers        {#mainpage}
====================================================================================================

# Introduction #                                                      {#AvrPlusIntroduction}

The AVRToolsPlus library provides a collection of functionality for the AVR
family of 8-bit microcontrollers.  Unlike the low-level functionality found in
the AVRTools library (which very directly interfaces with AVR hardware
subsystems), the functionality in the AVRToolsPlus library is more high-level
and does not relate directly to hardware subsystems.  In fact, the modules in
AVRToolPlus are sufficiently generic that they often don't even make use of
AVRTools functionality. Nevertheless, the code in AVRToolsPlus is specifically
intended for use on AVR 8-bit microcontrollers and the implementation often
relies on specific characteristics of this family of microcontrollers.

For this reason, rather add these modules to the AVRTools library, I decided to
package them in a separate library.



# Overview #                                                          {#AvrPlusOverview}

The AVRToolsPlus library includes a collection tools that provide higher-level
functionality.  These tools are designed for the AVR family of 8-bit
microcontrollers, and while the implementation of AVRToolsPlus modules depends
on characteristics of the AVR 8-bit architecture, these modules are not
intimately tied to any specific AVR hardware subsystems.  For this reason,
AVRToolsPlus modules should run on any of AVR's 8-bit microcontrollers (although
I have only tested them on ATmega328 and ATmega2560).

The tools in AVRToolsPlus are organized as a collection of independent modules.
These modules are:

- [EventManager module] (@ref EventManager)

The AVRToolsPlus modules do not depend on any of the AVRTools modules, but are fully
interoperable with AVRTools.



# Documentation #

Detailed documentation is provided by this PDF document located in the repository, or
[online in HTML form] (http://igormiktor.github.io/AVRToolsPlus/).



## Feedback

If you find a bug or if you would like a specific feature, please report it at:

https://github.com/igormiktor/AVRToolsPlus/issues

If you would like to hack on this project, don't hesitate to fork it on GitHub.
If you would like me to incorporate changes you made, please send me
a Pull Request.

