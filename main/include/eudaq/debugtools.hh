#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

namespace eudaq{

/// A dummy replacement for an lcio event.
class lcioevent
{
public:
    int a;
    lcioevent() : a(0) {}
};

}

#endif// DEBUGTOOLS_H
