#Senior Capstone Project
### Problem statement
There is a lack of inexpensive multifaceted image recording drones, especially for recreational users who do not see the value in buying a very expensive drone. A user may want to capture an experience, landscape, or market in a different way, and therefore we developed the 3-Dimensional Anaglyph Drone (3DAD). It is a drone camera attachment that can take a pair of 2-D images, controlled by the user from the ground. Part of this product is a custom image-processing software solution, which manipulates the acquired 2-D data to generate a true 3-D anaglyph image.
There is also no easy way to modify a drone with a camera attachment for the average user, and existing solutions are much more expensive than the original product. To enhance the 3DAD, the cameras can rotate independently from the drone in unison and can also tilt downwards for a true birds-eye view of the ground by using a remote control built specifically for camera control.

### 3DAD Image Viewer
Purpose: Allows a user to view and apply filters on a set of two 2-Dimensional images obtained from a stereo camera set-up and output a 3-Dimensional anaglyph image.
Description: Developed in C# utilizing library AForge.NET, an open-source C# framework for developers in Computer Vision.

Main Features:
- Re-coloring, linear correction, convolution,and correction filters
- Produces 3-D anaglyph real-time as filters are applied
- Produces sobel transform image real-time as filters are applied
- Produces difference image of filtered images real-time as filters are applied
- Allows user to save filtered 2-D images, sobel image, difference image, and anaglyph image
- Displays RGB histograms (and mean,median,std dev values) for each 2-D image that adjusts real-time as filters are applied
