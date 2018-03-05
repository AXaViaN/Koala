Coding Standards
================

.. contents:: :local:

Following same rules everywhere in the project makes the code easy to read and easy to maintain. This section describes the coding conventions being followed in the project as well as some of the best coding practices. All of the contributors to the project are expected to follow these rules.

Naming Conventions
------------------

- Variable, method, and class names should be clear, unambiguous, and descriptive. There should be no abbreviations.

- Function and method names are verbs. Type and variable names are nouns.

- The first letter of each word in every function/method and constant name is capitalized and there is no underscore between words. This rule is valid on variable naming with some exceptions:
	
	- Variable names in function bodies and function parameters starts with a lower-case letter.
	
	- Global variables starts with :code:`g_` prefix. E.g. :code:`static Logger g_DebugLogger;`
	
	- Variable names in classes/structs are treated differently by their visibility and specifiers:
		
		- All public variables follow the same rule with methods, except one letter names. E.g. :code:`float Node::Weight`, :code:`float Vector3::x`
		
		- Protected variables starts with :code:`p_` prefix, indicating protected.
		
		- Private variables starts with :code:`m_` prefix, indicating member.
		
		- Static variables on private fields starts with :code:`s_` prefix, indicating static.

- Method names describes the method's effect, or describe the return value of a method that has no effect. The name should make clear what value the function will return. This is particularly important for boolean functions. All functions that return a bool should ask a true/false question, such as :code:`IsNodeReady()` or :code:`ShouldClearBuffer()`. :code:`CheckNode()` would have created an ambiguity (what does 'true' mean?).

- All variables should be declared on their own line. This helps readability and enables to comment on the meaning of each variable separately.

Classes
-------

- Enforce encapsulation with the protection keywords. Class members should almost always be declared private unless they are part of the public/protected interface to the class. If particular fields are only intended to be usable by derived classes, make them private and provide protected accessors. Use final if your class is not designed to be derived from.

- Class visibility should be organized with the API user in mind. API user only needs to know public and protected fields. Private fields should be at the bottom. Methods, constants and variables are also seperated even if they are in the same visibility level. Following shows this structure with optional fields:

	.. code-block:: c++
		
		class ClassName
		{
		// friendship
		
		public:
		    // subclass/struct/enum declaration
		
		public:
		    // methods
		
		public:
		    // constants
		
		public:
		    // variables
		
		protected:
		    // Same as public
		
		private:
		    // Same as public
		
		};

- Use the virtual and override keywords when declaring an overriding method. Although C++ permits overriding without these keywords, it creates confusion and makes it hard to understand if the method is just a member or it is from parent class.

	.. code-block:: c++
		
		class A
		{
		public:
		    virtual void F() {}
		};
		
		class B : public A
		{
		public:
		    virtual void F() override;
		};

- Do not use protected static members. That makes it harder to trace the values. Instead, make them private and provide a protected getter.

- Information about the method, its parameters and use cases are documented in the API Documentation. So headers do not need commenting. However, details about the implementation of a method should be commented within the method implementation.

- All headers should protect against multiple includes with the include guards. To eliminate macro collision, use upper-case letters and use module/folder names with two underscores inbetween. If the class is in a private folder, add :code:`PRIVATE__` in the beginning.

	.. code-block:: c++
		
		/* 
		 * Koala/Editor/CodeBoard.h
		 */
		
		#ifndef KOALA__EDITOR__CODE_BOARD
		#define KOALA__EDITOR__CODE_BOARD
		
		// ...
		
		#endif // KOALA__EDITOR__CODE_BOARD
		
		/* 
		 * Koala/Editor/Private/CodeBoard.h
		 */
		
		#ifndef PRIVATE__KOALA__EDITOR__CODE_BOARD
		#define PRIVATE__KOALA__EDITOR__CODE_BOARD
		
		// ...
		
		#endif // PRIVATE__KOALA__EDITOR__CODE_BOARD

- Some modules have Private source directories. Any definitions that are needed by other modules must be in headers in the module directory, but everything else should be in the Private directory.

- Prefer composition over inheritance. Inheritance is necessary in some cases, but using them too much starts to cause problems. If inheritance is needed, never use multiple inheritance and do not make inheritance tree deeper than 1 (never inherit a child class).

- If you are using any of the custom destructors, copy/move constructors or copy/move assignment, apply rule of five. You can use a separate public field to clear API from cluttering.

General Rules
-------------

- Always use the latest C++. But be aware that a new feature may not available on all compilers. Use features that are well supported on variety of compilers. Refrain from using compiler-specific language features unless they are wrapped in preprocessor macros and used sparingly.

- Declare variables where its needed, not at the top of an execution block. When code depends on a variable having a certain value, try to set that variable's value right before using it. Initializing a variable at the top of an execution block, and not using it for a hundred lines of code, gives lots of space for someone to accidentally change the value without realizing the dependency. Having it on the next line makes it clear why the variable is initialized the way it is and where it is used.

- Do not use magic numbers. Use descriptive constants to make code easy to understand without comments.

	.. code-block:: c++
		
		// Bad - Why are we multiplying with that number?
		width = windowWidth * 0.8333333f;
		
		// Good - We are calculating the frame width
		constexpr float FrameScale = 0.8333333f;
		width = windowWidth * FrameScale;

- All code should strive to be const-correct. This includes passing function arguments by const pointer or reference if those arguments are not intended to be modified by the function, flagging methods as const if they do not modify the object and using const iteration over containers if the loop isn't intended to modify the container. You can find more information about this idiom at `here <https://isocpp.org/wiki/faq/const-correctness>`_.

- In general, try to minimize physical coupling. If you can use forward declarations instead of including a header, do so. Do not rely on a header that is included indirectly by another header you include. Do not rely on being included through another header, include everything you need.

- Split up large functions into logical sub-functions where possible. It is easier to understand a simple method that calls a sequence of several well named sub-methods than to understand an equivalent method that simply contains all the code in those sub-methods.

- Use inline functions judiciously, as they force rebuilds even in files which don't use them. Inlining should only be used for trivial accessors.

- Never pollute the global scope, never use :code:`using` declarations. Everything is inside the project namespace. Every module should have their own namespaces. E.g. :code:`namespace Koala::Utility` for *Koala/Utility* folder.

- Address compiler warnings. Compiler warning messages mean something is not as it should be. Compilation should run without any warning messages.

- Leave a blank line at the end of all .cpp and .h files. If the project will be compiled with a different compiler in the future, some compilers may complain about this.

- Debug code should either be generally useful and polished, or not checked in. If it needs to stay inactive, it should be surrounded with :code:`#if 0`/:code:`#endif` block, instead of a comment block.

- Use intermediate variables to simplify complicated expressions. It will be easier to understand if you split it into sub-expressions that are assigned to intermediate variables with names describing the meaning of the sub-expression within the parent expression. For example:

	.. code-block:: c++
		
		// Bad - Hard to understand what is this checking for
		if(node.IsActive() && 
		   newConnection == nullptr && 
		   selection.Index != 0 && 
		   selection.IsLeftDown && 
		   selection.IsFlowBack)
		{
		    DoSomething();
		}
		
		// Good - With descriptive names and grouping, checks are more clear now
		bool isNodeAvailable = node.IsActive() && newConnection == nullptr;
		bool isSelectionValid = selection.Index != 0 && selection.IsLeftDown && selection.IsFlowBack;
		if(isNodeAvailable && 
		   isSelectionValid)
		{
		    DoSomething();
		}

- :code:`nullptr` should be used instead of the C-style :code:`NULL` macro in all cases.

- You should only use :code:`auto` in some specific cases. When you are using :code:`auto`, always remember to correctly use :code:`const`, :code:`&` or :code:`*` just like you would with the type name. This will make the inferred type to be what you want. Allowed use-cases are:

	- For iterator variables, where the iterator's type is very verbose and would impair readability.
	
	- In template code, where the type of an expression cannot easily be discerned.
	
	- When you need to bind a lambda to a variable, as lambda types are not expressible in code.

- Range based for is preferred to keep the code easier to understand and more maintainable. Whenever you are iterating over an array and you don't need index, use it.

- In lambda expressions, prefer explicit capture, especially for large lambdas and deferred execution. Automatic capture (:code:`[&]` and :code:`[=]`) can cause dangling references, unnecessary copies and unintentional capturing of :code:`this` pointer.

- Enum classes should always be used instead of old-style enums. It prevents pollution and it enables type safety.

	.. code-block:: c++
		
		// Old enum
		enum Color
		{
		    Red,
		    Green,
		    Blue
		};
		
		// Enum class
		enum class Color
		{
		    Red,
		    Green,
		    Blue
		};

- Never use exception handling. Instead, return error codes on failures and manage stack unwinding manually.

Code Formatting
---------------

Braces { }
**********

Curly braces always put on a new line (except namespaces). Always include braces in single-statement blocks.

	.. code-block:: c++
		
		void Foo()
		{
		    if(condition)
		    {
		        return;
		    }
		    
		    // ...
		}
		
		namespace Koala {
		
		// ...
		
		}

If a method body will be left blank, braces will be on the same line with a space in between.

	.. code-block:: c++
		
		class Child : public Parent
		{
		    // ...
		    
		    virtual void Foo()
		    { }
		
		};

If - Else
*********

Conditions should be written with spaces on both sides of the operator. If a condition is expected to be :code:`true` and the variable or the method is already in a true/false question form (e.g. :code:`IsNodeReady()`), writing :code:`== true` could be omitted. But if a condition is expected to be :code:`false`, instead of writing :code:`!condition`, you should write :code:`== false` explicitly. If there will be multiple conditions with :code:`&&`/:code:`||` operators, every expression needs to be on their own line, indented with the first expression.

	.. code-block:: c++
		
		if(condition1 && 
		   condition2 == false)
		{
		    DoJob();
		}
		else if(condition3 > 0 || 
		        condition4 < 0)
		{
		    DoOtherJob();
		}
		else
		{
		    WaitForJob();
		}

Loops
*****

:code:`while` loop conditions works same as the :code:`if` conditions. :code:`for` loop syntax is as follows:

	.. code-block:: c++
		
		for( int i=0 ; i<100 ; ++i )
		{
		    // ...
		}
		
		for(  ; x>10 && x<100 ; ++x )
		{
		    // ...
		}

If the condition is too long, use an intermediate boolean.

Tabs vs Spaces
**************

Use tabs, not spaces, to indent code by execution block. Set your tab size to 4 characters. However, spaces are sometimes necessary and allowed for keeping code aligned. In those cases, use tabs until you need spaces (so that space count will be <4).

Switch Statements
*****************

Every :code:`case` should have curly braces and a :code:`break` at the end. Other code control-transfer commands (:code:`return`, :code:`continue`, etc.) are fine too. But never do fall-through; abstract the common code to a function and call that from both places. Empty :code:`case` blocks (multiple :code:`case` blocks having identical code) should be on their own line, one after another.

	.. code-block:: c++
		
		switch(condition)
		{
		    case 1:
		    {
		        // ...
		        
		        break;
		    }
		    case 2:
		    {
		        // ...
		        
		        return;
		    }
		    case 3:
		    case 4:
		    {
		        // ...
		        
		        break;
		    }
		}

Classes
*******

Always specify the base class visibility. Default variable initialization should be done in variable definitions. Syntax is as follows:

	.. code-block:: c++
		
		class ClassName : public BaseClass
		{
		public:
		    ClassName(int v0, int v1, int v2) : 
		        BaseClass(v0), 
		        
		        m_Var1(v1), 
		        m_Var2(v2)
		    { }
		    
		private:
		    int m_Var1;
		    int m_Var2;
		    int m_Var3 = 0;
		    
		};

Namespaces
**********

Namespace names should be commented in the closing brace to clear any possible confusion. For nested namespaces, use the C++17 syntax.

	.. code-block:: c++
		
		namespace Koala {
		
		class Main
		{
		    // ...
		
		};
		
		} // namespace Koala
		
		namespace Koala::Editor {
		
		// ...
		
		} } // namespace Koala::Editor

Includes
********

Never do includes relative to the current source file. That makes it harder to move headers around. Instead, set the Source folder as include directory and use :code:`#include <Koala/File.h>` syntax. Since we are using the project name as base folder, we don't need :code:`#include "Koala/File.h"` syntax to make a distinguishment.

Sort included files for easy readability. First, include the own header of the source, then include project headers, then external libraries.

General Style Issues
********************

- In function declarations or function call sites, do not add a space between the function's name and the parentheses that precedes the argument list.

- Pointers and references should only have one space, which is to the right of the pointer/reference.

	.. code-block:: c++
		
		// Not these
		Resource *resource;
		Resource * resource;
		
		// This
		Resource* resource;

- Never use shadowed variables. C++ allows variables to be shadowed from an outer scope. That makes the usage ambiguous to a reader. For example:

	.. code-block:: c++
		
		void Foo(int bar)
		{
		    for( int bar=0 ; bar<10 ; ++bar )
		    {
		        // Using bar here could confuse the reader.
		        // It could lead to think that it is the function parameter.
		    }
		}

- In lambda expressions, if there is a return type, always specify the return type explicitly. Syntax:

	.. code-block:: c++
		
		auto calculateSum = [](int x, int y) -> int
		{
		    return x+y;
		}
		
		int sum = calculateSum(5, 10);

- If an :code:`#if` block is long, comment the performed check at the :code:`#endif` statement. Same applies for namespaces.

	.. code-block:: c++
		
		#ifdef FOO__BAR
		
		// Long code
		
		#endif // FOO__BAR
