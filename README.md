# LLV

> A linked list (and way more!) visualisation tool

> Made for teaching purposes for UNSW CSE

> Made by Braedon Wooding

Originally this project just supported linked lists, now it supports (hypothetically) any collection at all!  Currently we support;

- Singularly Linked Lists
- Doubly Linked Lists
- Vectors (dynamic arrays)
- Arrays (static)
- Queues and Stacks

In the future we are planning to support

- Various types of trees (BST/Tries)
- Various types of graphs
- Hashtables/Hashsets

Also note that the below image has the tick time quite low this is just to make it look nice as a gif, so you'll probably want to up that to make it so you can follow it, if set to 0 then it'll use manual stepping (enter to make each update pass)

![Demonstration](https://user-images.githubusercontent.com/22880786/46268789-0084f780-c580-11e8-9278-ca123f8ba489.gif)

## How to install

- I suggest you just download the library from the releases section in github
  - This is mainly due to stability reasons
  - Just download the `download_these` files, it comes with the required include headers and the compiled library.
- To compile from source just clone (or download zip I guess) then just run `cmake .` to produce the required makefiles for your system, then just `make` should produce the library.  You'll still have to include the lib headers.
- To compile with your file just do something like `gcc libLLV.a my_file.c` you can also add an `-I ~/LLV` (presuming you called it LLV and it is installed in home directory) so you can just do `#include <LLV/collections/Array.h>` if you want

## For those wanting to build a new collection

You just build it like you normally would however make sure the alignment of the first few types match the alignment of `Collection`, this is just so we can have effective generics behind the scenes for our printer, just makes it easier (since we need to access some of the fields).

Furthermore if it is similar to one of the current collections there is a pretty good chance I've already made the core printing logic under collection_helper, as long as your nodes match up with the `FakeNode` alignment then they will work, they take the barebones implementation required to print them out.  For example DLL and LL both use the same logic to print out their nodes.

## Quick contributors guide

I would love for you to help maintain this, just a few things to consider;

- We use [todo](https://github.com/BraedonWooding/Todo) for our current list of items
  - You don't have to use this of course :), it is just a way for me to keep track of things.
  - Currently a little buggy as I haven't updated it in a short while but I'll get around to it.
- Small changes over big
  - Big changes are sometimes necessary, I've re-written the core once already
    since I wasn't happy with the lack of extensibility for other things than
    just a linked list.  But this won't happen again so if it is > 100 lines maybe it can be broken up a bit?
    - Exception would be new collections (though one collection per PR would be nice)
- Currently the core code base is quite trivally small (~1k lines not including examples)
  - Overall it is around 2.5k and will grow exponentially as new tests and collections come however if you remove tests and collections it is around 1k lines which is what I want it to be around, I do expect it to creep up to 2k but I don't want it to become too unmanageable.
- Testing is pretty easy with all the stuff I've done :).
  - Testing collections is done via obsidian
  - Testing the actual program however is done through `output_tests` which contains two files for each test, a source file to run and expected output.
    - To produce a test I suggest you first go through it manually making sure each 'screen' is valid then run `./my_program.out > my_test.expected` instead of trying to build expected output manually, this kinda invalidates TDD but I don't really think it can be done another way (if you have any ideas I would love to hear them).
    - DO NOTE: that when making a test design it around the screen size being 80 in it's width you can define a different default size if you wish `#define DEFAULT_TERMINAL_WIDTH (80)` this width currently only effects tests, GDB/LLDB and other cases where we can't get the terminal width normally.
- Style guide (below)
- Be nice to everyone :)
- Label your PRs / Issues with `[<label>]` i.e. `[Bug]` (for issue) or `[Small]` (for PR)
  or `[Bug Fix]` (for PR) just so I have an idea at a glance what you are doing.
  - No 'requirements' on what they are called, but something like `[Collection]` for new collections would be nice!

## Roadmap

- Support the collections stated at the top of this readme
- Have `#ifndef`'s for most of the defines so you can modify them
  - For example the width of the array can be set to -1 to have them be next to each other i.e. `|a|b|c|` and no extra bars or set to 0 to have extra bars i.e. `|a||b||c|` or can be set to any positive number to have spaces i.e. `|a| |b| |c|` this would be nice to set outside of having to edit source.
- Much more tests! (break a few things!!!)

## Quick Style Guide

```C
// struct names are snake lowercase and begin with a `_` and end with a `_t`
struct _struct_name_t {
    int x; // member names are all lower case
};
// note: no common names, this is to prevent bugs where you do something like
// also use malloc_with_oom, just so we are clear when we oom.
// MyObj my_obj = malloc_with_oom(sizeof(my_obj), "MyObj");
// where my_obj is the common name of MyObj (non ptr name)
// which seems fine until you realise it just mallocs a pointer since it proritises
// the variable over the 'common' name!
// The ONLY case I'll accept common names are if the class is not to be malloc'd
// i.e. look at `terminalSize`

// pointer types are upper camel case (pascal case)
typedef structName *StructName;

int foo_boo_wow(int bar) {
    int magic_number = 42;
    return magic_number;
}

// Scoped methods to class
int stack_pop(Stack s) {
    // ...
}

// Switch statements are to be in this format (prevents bugs)
switch (x) {
    case a: {
        // ...
    } break;
    case b: {
        // ...
    } break;
}
```
