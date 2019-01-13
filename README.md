# LLV

[![Build Status](https://travis-ci.com/BraedonWooding/LLV.svg?branch=master)](https://travis-ci.com/BraedonWooding/LLV)

> A generic collection visualisation tool

> Originally made for teaching purposes for UNSW CSE

> Made by Braedon Wooding

![Demonstration](https://user-images.githubusercontent.com/22880786/51027673-9d2a4d00-15e5-11e9-9ead-787631546460.gif)

> The tick rate is set to be extremely fast (otherwise it wouldn't fit within the gif limits) so normally you would either set it to be much higher (higher is slower) or you would set it to 0 which would mean you press enter to step manually between each animation frame.

## NOTE

Check out [Quick Start Guide](https://github.com/BraedonWooding/LLV/wiki/Quick-Start)

Originally this project just supported linked lists, now it supports (hypothetically) any collection at all!  Currently we support;

- Singularly and Doubly Linked Lists
- Dynamic and static arrays (arraylist/vector/list and arrays)
  - We call our dynamic arrays just `List` since `Vector` is too opaque and `ArrayList` is too long and we don't have to worry about conflictions with linked lists since they are `LL` and `DLL` respectively.
- Queues and Stacks

In the future we are planning to support

- Various types of trees (BST/Tries)
- Various types of graphs
- Hashtables/Hashsets

Also note that all collections aren't singularly typed so you could mix int/float/string/void* within a single collection!  (though typically it is recommended to keep to one).

## Other features

- We support unicode and 'ascii' (all tests run on both) unicode just makes the boxes look nicer and the arrows are less ascii like.
- We support changing variables without requiring re-compiles (especially important since the core library is meant to not have to be ever recompiled) these include; disabling unicode, changing default dimensions, changing step time...  Look at [Changing Variables](https://github.com/BraedonWooding/LLV/wiki/Reference-Sheet#variables) for more.
- You can take input during it and we do all the type conversions for you!

## How to install

- Just run `wget https://raw.githubusercontent.com/BraedonWooding/LLV/master/install.sh && sh install.sh` on any posix system (i.e. cse, MacOS, Linux/BSD variants)
  - or if you are on windows just copy + paste `include.zip` contents into your include path and `libLLV.a` into your lib path from the latest release [here](https://github.com/BraedonWooding/LLV/releases)
- Then just have `#include <LLV/llv.h>` and any collections you want like `#include <LLV/collections/ll.h>`
  - `ll.h` (Linked List), `dll.h` (Doubly Linked list), `array.h` (Static Array), `list.h` (Dynamic Array), `queue.h` (Queue LILO/FIFO), `stack.h` (Stack FILO/LIFO) are the currently supported collections.
- Note: if you are having issues with anything check out the [FAQ](https://github.com/BraedonWooding/LLV/wiki/FAQ) first before opening an issue

## For those wanting to build a new collection

You just build it like you normally would however make sure the alignment of the first few types match the alignment of `Collection`, this is just so we can have effective generics behind the scenes for our printer, just makes it easier (since we need to access some of the fields).

Furthermore if it is similar to one of the current collections there is a pretty good chance I've already made the core printing logic under collection_helper, as long as your nodes match up with the `FakeNode` alignment then they will work, they take the barebones implementation required to print them out.  For example DLL and LL both use the same logic to print out their nodes.

## Quick contributors guide

I would love for you to help maintain this, just a few things to consider;

- Small changes over big
  - Big changes are sometimes necessary, I've re-written the core once already
    since I wasn't happy with the lack of extensibility for other things than
    just a linked list.  But this won't happen again so if it is > 100 lines maybe it can be broken up a bit?
    - Exception would be new collections (though one collection per PR would be nice)
- Testing is pretty easy with all the stuff I've done :).
  - Each output test is run under the following conditions; ascii/unicode normal boxes, ascii/unicode small boxes, ascii/unicode large boxes, small_term ascii/unicode.
  - To run tests you just do `make run_tests`
  - Testing collections is done via obsidian
  - Testing the actual program however is done through `output_tests` which contains two files for each test, a source file to run and expected output.
    - There is a test helper called `generate_matrix_output.sh` that will run all the given files under all test cases and produce the expected output for you
  - All tests are run on every commit via Travis CI this way we can help stop regression issues.
  - Note: DLL unicode expected output tests may look a little funny due to the fact that ⟺ is larger in coding fonts than terminal ones.
- Style guide (below)
- Be nice to everyone :)
- Label your PRs / Issues with `[<label>]` i.e. `[Bug]` (for issue) or `[Small]` (for PR)
  or `[Bug Fix]` (for PR) just so I have an idea at a glance what you are doing.
  - No 'requirements' on what they are called, but something like `[Collection]` for new collections would be nice!

## Roadmap

- Support the collections stated at the top of this readme
- Much more tests! (hopefully breaking a few things!!!)
- Memory tests to make sure we are freeing stuff properly
  - This is much lower priority though

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
// i.e. look at `terminalSize` in that case they can't have a uppercase pointer name

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

// Switch statements are to be in this format
// (mainly because this way it is much easier to see forgotten breaks)
switch (x) {
    case a: {
        // ...
    } break;
    case c: case d: {
        // ...
    } break;
    case q: {
        // ...
    } // fallthrough to p (fallthroughs like this should be avoided)
    case p: {
        // ...
    } break;
}
```
