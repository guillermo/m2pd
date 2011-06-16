
Mongrel 2 Puredata Handler
==========================

Mongrel 2 puredata Handler is puredata library that acts as a web server through Mongrel 2 and zmq. So you can manage PD through a webbrowser.

When the objet is created, it connects to the mongrel sockets tcp://localhost:9999 as a SUBSCRIBER and tcp://localhost:9998 as a PUBLISHER.


TODO
====

* Make it threaded, so no metronome (metro) is needed.
* Validate messages format.
* Output list instead of symbols.
* Create a sample web page frontend.
* Allow output from pd to be serverd to many clients (broadcast) through mongrel 2.


MOTIVATION
==========

Make a good online radio, and easy to update. I don't found any easy program to create a radio with crossfade (and beat sync), singles, ads supports, etc...


AUTHOR
======

Guillermo Álvarez Fernńadez <guillermo@cientifico.net>


LICENSE
=======

Beerware 
