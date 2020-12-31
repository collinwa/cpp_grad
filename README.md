# CPP Grad -- A pedagogical implementation of autograd

Did the world need another implementation of auto-differentiation?

...


no.

However, we have decided to build one anyways. Meet cppGrad, a reverse-mode autodifferentiation 
engine that works on individual Values. There are some quirks to the interface that we'll
document (mostly, choices that were made to make it easier on us). While of little _practical_
use, we hope that this can at least be used for the sake of learning.
