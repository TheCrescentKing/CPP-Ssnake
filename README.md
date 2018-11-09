# CPP-Ssnake
My first 'independent' project made after my first semester at university Lyon 1 (UCBL)
[4 months of uni coding classes in C++ & algorithms & math].

Built using the library "GrAPiC" (http://liris.cnrs.fr/alexandre.meyer/grapic/html/index.html).

Last tested on Windows with Grapic 1.0.50 using Code::BLocks.
Instruction for setting up the library on the above site.

![Ssnake.png](https://i.imgur.com/3tTgLAu.png)

The above screenshot is during the mode where after you 'eat' a star, the colours are randomized in a Mario-Kart inspired way.


Video:

[![IMAGE ALT TEXT](http://img.youtube.com/vi/4ptvsWDsG94/0.jpg)](https://youtu.be/xO-whE5Uzyo "Ssnake Project")

Instructions:
Once you have downloaded and set-up Grapic, merge the data folder with the one in Grapic. Then add the Ssnake folder to grapic-_version_/apps.
Lastly, modify the "grapic.lua" file after the line that says "-- description des projets" with:

make_project("Ssnake", "apps/Ssnake/main_Ssnake.cpp")
