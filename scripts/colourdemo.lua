function setup()

-- funky
funky = newTexture(128, 128);
filledRect(funky, 20, 20, 30, 100, 240, 40, 40);
filledRect(funky, 98, 20, 108, 100, 240, 40, 40);
filledRect(funky, 20, 70, 108, 50, 240, 40, 40);
applyEdgeDetect(funky, 0, 1, 1, 1, 1);
quantize(funky, 2, 2, 2);
--fill(funky, 3, 3, 3, 0, 0);
setVStrip(funky, 30, 80, 64, 146, 198, 140);
setPixel(funky, 10, 10, 255, 0, 0);
setPixel(funky, 10, 118, 255, 0, 0);
setPixel(funky, 118, 10, 255, 0, 0);
setPixel(funky, 118, 118, 255, 0, 0);
closestColour(funky, 1.0, 0, 0, 0);
makeFinal(funky);

-- linesplat
lsplat = newTexture(128, 128);
setHStrip(lsplat, 30, 68, 20, 255, 0, 0);
setHStrip(lsplat, 30, 68, 40, 200, 100, 50);
setHStrip(lsplat, 30, 68, 60, 50, 100, 200);
setHStrip(lsplat, 30, 68, 80, 100, 140, 10);
setHStrip(lsplat, 30, 68, 100, 210, 90, 222);
setVStrip(lsplat, 10, 108, 64, 200, 200, 0);
closestColour(lsplat, 1.0, 0, 0, 0);
makeFinal(lsplat);
end