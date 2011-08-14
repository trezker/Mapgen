mapgen readme
v1.01

1. Introduction
2. Usage
3. Contact
4. Notes for the interested


1. Introduction
Mapgen is a map generator using a fractal algorithm to create beautiful maps.
The generator creates a heightmap first  which means a two dimensional array containing height values.
Then the map is coloured using a coloursystem and, if selected, raytraced shadows.
I'm not going into any deeper explanation here.

Mapgen was started by Johan Halmén, originally it was a contribution to a contest in pixelate (pixelate.co.za).
He did a couple of fixes after that but it ended up on ice, left to collect dust.
Then I, Anders "Trezker" Andersson, wanted to make a certain game, and this mapgenerator seemed to be usable.
Though it needed quite a brushup to meet my needs and after consulting Johan I started working on it.
The game is still in my dreams only, but mapgen got it's brushup.



2. Usage
Most features should be self explanatory, so I'm just mentioning some details you might need.
Azimuth means the compass direction to the sun.
Altitude means sun height from horizon.

Try different fractal factors by selecting the radio button that says 1.6 by default and edit that text.

Shadows are off by default because they slow down rendering incredibly.
When you find an interesting map you can turn shadows on and render current key.

You can view a rendered map in 3D, but be aware that this feature has some bugs and might crash the program.
This isn't fatal, it just means you can't view all maps in 3D, especially the big 512 maps.
You can choose to flatten the map below and above given values.
These values depend on what palette you are using, if you have sea it should be flat, and you might have plateumountains.
These values should be between 0 and 1, since that's the range of the palette used to render the map.
I have the lower flattening activated by default due to the sealevel at 0.5 in the terrain palette.



3. Contact
Both me and Johan can be reached through allegro.cc



4. Notes for the interested
v0.5 release notes said
If you want to know more about the algorithm for the map generating,
download the earlier package and read the readme.

Trezker's explorations
I haven't found an X map, but I've found a + map, try seed 9403.