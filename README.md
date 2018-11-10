# LLV

> A linked list (and more!) visualisation tool

> Made originally for teaching purposes for UNSW CSE

> Made originally by Braedon Wooding

Originally this project just supported linked lists, now it supports (hypothetically)
any collection at all!  Currently we just support;

- Singularly Linked Lists
- Doubly Linked Lists
- and very soon Queues/Stacks

In the future we are planning to support

- Various types of trees (BST/Tries)
- Various types of graphs
- Hashtables...

Also note that the below image has the tick time quite low this is just to make it look nice as a gif, so you'll probably want to up that to make it so you can follow it, if set to 0 then it'll use manual stepping (enter to make each update pass)

![2018-10-01 13 37 58](https://user-images.githubusercontent.com/22880786/46268789-0084f780-c580-11e8-9278-ca123f8ba489.gif)

## How to install

- I suggest you just download the library from the releases section in github
  - These are more stable and are much easier to use
  - They come in just a single fromat `libLLV.a` which is just a static library
    - Note: these libraries are actually (pseudo) compiled so they have architecture make sure you get the right one
- If you want to compile from source all you need is cmake, then just say `cmake ./` in the home directory
  - Then you will have a ton of make files and stuff pop up (we don't commit these auto generated files)
  - To compile the library now all you have to say is `make`
- To use the library you just use it like any other C library i.e. `gcc myFile.c libLLV.a -o outFile`

## Small things to consider

- Lists are relatively expensive in terms of memory
  - So this visualization tool isn't really suited towards creating a lot of lists
    more around doing a lot of manipulation of a few lists.
  - Nodes are relatively cheap
    - They do carry a forwards pointer always and a pointer to a string for display
      - However I am investigating other ways we can have visual ptrs that aren't
        as ugly as the ones in the past were (that is not visually but programatically).

## How does this work

Just incase you are interested;

- We effectively emulate inheritance by using a pseudo v-table.
- This gives us a lot of wliggle room with having independent printing options
  - And each one also allows you to customize how you want to print the node.
- Effectively each print just calculates the sizeof the list and adjusts in the case when it won't fit.
  - By adding a '...', this operation is relatively complex since each node can have an arbitary size
  - So what we have to do is effectively go both forward and backward
  - Trying to find a semi-optimal solution to show the maximum amount of nodes
    - We don't care about it being too overly optimal, just close enough
      - Overtime I'll improve it, currently its a little too noticeably unoptimal for my tastes
- It also just prints the list out to a buffer which is actually N buffers long
  each one representing the a level of the buffer, this allows us to print out nodes
  one at a time rather than having to print out the tops of each one then the centres
  then the bottoms... which made the code a WHOLE lot simpler.

## For those wanting to build a new collection

You just build it like you normally would however make sure the alignment of the first few types match the alignment of `Collection`, this is just so we can have effective generics behind the scenes for our printer, just makes it easier (since we need to access some of the fields).

Furthermore if it is similar to one of the current collections there is a pretty good chance I've already made the core printing logic under collection_helper, as long as your nodes match up with the `FakeNode` alignment then they will work, they take the barebones implementation required to print them out.  For example DLL and LL both use the same logic to print out their nodes.

## Quick contributors guide

- We use [todo](LINK_NEEDED) for our current list of items to well 'todo'
  - You don't need to use it you can just update the TOML (which is soon changing to md)
- Small changes over big
  - Big changes are sometimes necessary, I've re-written the core once already
    since I wasn't happy with the lack of extensibility for other things than
    just a linked list.
- Currently the code base is quite trivally small (~1k lines not including examples)
  - I would prefer for it to remain trivally small, I don't mind if there are tons
    of collections, but having the codebase be anything over 3k lines can become
    more tedious to manage and maintain, especially since testing with this kind
    of application is quite hard.
- Since testing is hard...
  - Well its actually not too insane, I have a few plans to pull out where it prints to
    and have that as a customizable variable (at compile time) so we can easily get
    it printing properly to a log without having to do bash redirects which can cause problems
    with inputs, even though we will probably just be setting the wait time to 1 (the smallest
    delta you can have) so it is not like the program will receive inputs.
    - I can't really get testing working now though since with how it places nodes
      aren't at all consistent, once I fix that (should just require some tweaking)
      we will be in a consistent place to predict exactly how nodes shall be placed.
    - Note: this consistency is not about 'random' but more about it having a too large
      buffer zone for it to place its nodes, but this buffer zone is required for some
      inputs, so we have to figure out when its required and when not and deapply it
      such that its more accurate.
- Follow the style guide
- Be nice to everyone :)
- Label your PRs / Issues with `[<label>]` i.e. `[Bug]` (for issue) or `[Small]` (for PR)
  or `[Bug Fix]` (for PR) just so I have an idea at a glance what you are doing.
  - No 'requirements' on what they are called, but something like `[Collection]` for
    new collections would be nice!

## Quick Style Guide

```C
// struct names are snake lowercase and begin with a `_` and end with a `_t`
struct _struct_name_t {
    int x; // member names are all lower case
};
// note: no common names, this is to prevent bugs where you do something like
// MyObj my_obj = malloc(sizeof(my_obj));
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

// Scoped since is effectively a 'class'
int Stack_pop(Stack s) {
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
