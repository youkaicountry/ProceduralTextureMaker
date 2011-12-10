function setup()
handle = newTexture(128, 128);
line(handle, 10, 10, 25, 50, 255, 0, 0);
rect(handle, 10, 10, 25, 50, 255, 0, 0);
circle(handle, 64, 64, 16, 0, 255, 0);
colourReplace(handle, 0.0, 255, 0, 0, 200, 0, 200);

handle2 = newTexture(128, 128);
circle(handle2, 30, 30, 15, 100, 100, 10);
overlay(handle, handle2, 0.0, 0, 0, 0);

makeFinal(handle);
end