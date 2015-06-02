ofxHoa
================

ofxHoa is a wrapper of the [Hoa Library](https://github.com/CICM/HoaLibrary-Light "Hoa Library") (High Order Ambisonics), a set of C++ classes for sound spatialization in 2D and 3D based on Ambisonics. The current versions of the HoaLibrary support MacOs X, Windows and Linux. For openFrameworks we only support MacOs X. Windows and Linux are still a work in progress.

## IMPORTANT INSTALLATION NOTES

The ofxHoa uses a git submodule. If you choose to download it directly through the site,
the submodule may not be included.
Please check that you have the folder 

ofxHoa/libs/HoaLibrary-Light

and all of it’s contents. If not, download the HoaLibrary-Light in the following link:

https://github.com/CICM/HoaLibrary-Light

and put the /Sources folder inside ofxHoa/libs/HoaLibrary-Light

## Using the Library in openFrameworks

When using this library with oF, a few things must be taken into account.
Your speakers must be set in a circular or semi-spherical formation. For further information, check the the official site linked bellow.
When design your software, Hoa objects expect a traditional cartesian coordinate system centered around (0,0). If you do not take this into account, you’ll end up with a reversed Y axis and a source spacialized in relation to the top left corner of your screen.
It also considers that a source is inside the circle of speakers if it’s polar coordinate has a radius <= 1. Beyond that, there’s reduction of gain to emulate it’s distance.
If you use the math functions with the corrected cartesian coordinates, you’ll end up with a very low volume, as you’ll probably have radius values around 300 or more.
To avoid that, you must multiply the radius by the inverse of a radius in pixels that’ll correspond to your unity circle in screen. This may sound complicated, but everything is explained in detail and demonstrated in exampleOneSource.
Another thing to consider is that speakers are generally set at the same height in 2D sound, so the Y axis is not equal to the height of the source, but rather to it’s depth. 
So, in 3 dimensions the Z axis corresponds to it’s height. As a result, when dealing with 3D sound you must give the value of Z when the method asks for an ordinate and the value of Y when it asks for the height. This may seem confusing but it corresponds to common practices of sound.

The oscillator provided with this library was built for demonstration purposes only. It’s there to prevent the need for other addons in order to compile the examples.
Since the class ofxHoaOsc is quite CPU-intensive for a simple oscillator, We recommend to use this library with other addons such as ofxMaxim and ofxStk.

We hope this library you’ll bring you joy and help you in your work!

## Hoa Library 

#### A high order ambisonics library.

Sound space is one of the principal dimensions of the contemporary musical thought, specially in the electroacoustic music domain but also in intermedia arts. In this context, the <a title="CICM" href="http://cicm.mshparisnord.org/" target="_blank">CICM</a> has made spatialization its principal research axis. This project&#8217;s aim is to give to musician spatialization models based on high order ambisonics and sound fields synthesis. This project is developed in a part of the Paris 8 University <a title="CICM" href="http://www.labex-arts-h2h.fr/" target="_blank">LABEX arts H2H</a>. You can visit the official website : <a href="http://www.mshparisnord.fr/hoalibrary/" target="_blank"> HoaLibrary</a>.

![Image](https://raw.github.com/CICM/HoaLibrary/master/Ressources/hoa-icon.png "Hoa-Icon")

#### Authors :

Julien Colafrancesco, Pierre Guillot, Eliott Paris

#### Documentation :

<a title="Documentation" href="http://cicm.github.io/HoaLibrary/">documentation</a>

#### Dependencies : 

<a title="Cblas" href="http://www.netlib.org/clapack/cblas/" target="_blank">Cblas</a>, you can use Gsl, Apple Accelerate or Intel Composer XE.

##### License : 

The Hoa Library in under the <a title="GNU" href="http://www.gnu.org/copyleft/gpl.html" target="_blank">GNU Public License</a>. If you'd like to avoid the restrictions of the GPL and use Hoa Library for a closed-source product, you contact the <a title="CICM" href="http://cicm.mshparisnord.org/" target="_blank">CICM</a>.


