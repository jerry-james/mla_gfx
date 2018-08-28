This test emulates a screen virtually.  When requested to validate, it will dump the virtual screen to a .bmp and byte by byte validate it against a known good image.  The virtual screen will automatically resize based on the system/graphics configurations for width, height, and bpp.

NOTE: unlike other libraries, "correct" can be subjective in a graphics library.  If modifications are made to a routine that change the way in which an item on the screen is drawn (e.g. how much a pixel is alpha blended on a edge), then the expected result may also need to change.  

This project is build to run in the Netbeans IDE with the GCC compiler.