--reimplement this thing using adds (for fun)

function setup()

--SUISEISEKI
suiseiseki = newTexture(16, 16);
clearTexture(suiseiseki, 80, 80, 80);
xpad = 1;
--hat
setHStrip(suiseiseki, 3+xpad, 8, 0, 255, 255, 255);
setHStrip(suiseiseki, 2+xpad, 2, 1, 255, 255, 255);
setHStrip(suiseiseki, 10+xpad, 2, 1, 255, 255, 255);
setHStrip(suiseiseki, 2+xpad, 1, 2, 255, 255, 255);
setHStrip(suiseiseki, 11+xpad, 1, 2, 255, 255, 255);
setVStrip(suiseiseki, 4, 2, 3+xpad, 255, 255, 255);
setPixel(suiseiseki, 10+xpad, 4, 255, 255, 255);
setPixel(suiseiseki, 3+xpad, 7, 255, 255, 255);
setPixel(suiseiseki, 10+xpad, 7, 255, 255, 255);
setPixel(suiseiseki, 2+xpad, 8, 255, 255, 255);
setPixel(suiseiseki, 11+xpad, 8, 255, 255, 255);
--hair
setHStrip(suiseiseki, 4+xpad, 6, 1, 128, 60, 0);
setHStrip(suiseiseki, 3+xpad, 8, 2, 128, 60, 0);
setHStrip(suiseiseki, 2+xpad, 5, 3, 128, 60, 0);
setPixel(suiseiseki, 4+xpad, 4, 128, 60, 0);
setHStrip(suiseiseki, 8+xpad, 4, 3, 128, 60, 0);
setVStrip(suiseiseki, 4, 4, 2+xpad, 128, 60, 0);
setVStrip(suiseiseki, 4, 4, 11+xpad, 128, 60, 0);
setPixel(suiseiseki, 1+xpad, 9, 128, 60, 0);
setPixel(suiseiseki, 12+xpad, 9, 128, 60, 0);
setPixel(suiseiseki, 4+xpad, 11, 128, 60, 0);
setPixel(suiseiseki, 9+xpad, 11, 128, 60, 0);
setHStrip(suiseiseki, 1+xpad, 3, 12, 128, 60, 0);
setHStrip(suiseiseki, 10+xpad, 3, 12, 128, 60, 0);
setHStrip(suiseiseki, 0+xpad, 3, 13, 128, 60, 0);
setHStrip(suiseiseki, 11+xpad, 3, 13, 128, 60, 0);
setHStrip(suiseiseki, 0+xpad, 2, 14, 128, 60, 0);
setHStrip(suiseiseki, 12+xpad, 2, 14, 128, 60, 0);
--face
setVStrip(suiseiseki, 3, 5, 7+xpad, 253, 228, 176);
setVStrip(suiseiseki, 4, 4, 6+xpad, 253, 228, 176);
setVStrip(suiseiseki, 4, 4, 9+xpad, 253, 228, 176);
setVStrip(suiseiseki, 5, 2, 10+xpad, 253, 228, 176);
setVStrip(suiseiseki, 5, 3, 4+xpad, 253, 228, 176);
setHStrip(suiseiseki, 4+xpad, 6, 7, 253, 228, 176);
setPixel(suiseiseki, 3+xpad, 6, 253, 228, 176);
--eyes
setVStrip(suiseiseki, 4, 3, 5+xpad, 255, 0, 0);
setVStrip(suiseiseki, 4, 3, 8+xpad, 0, 255, 0);
--hands
setHStrip(suiseiseki, 2+xpad, 2, 11, 253, 228, 176);
setHStrip(suiseiseki, 10+xpad, 2, 11, 253, 228, 176);
--dress
setPixel(suiseiseki, 3+xpad, 8, 0, 190, 0);
setPixel(suiseiseki, 10+xpad, 8, 0, 190, 0);
setHStrip(suiseiseki, 2+xpad, 2, 9, 0, 190, 0);
setHStrip(suiseiseki, 10+xpad, 2, 9, 0, 190, 0);
setPixel(suiseiseki, 1+xpad, 10, 0, 190, 0);
setPixel(suiseiseki, 12+xpad, 10, 0, 190, 0);
setHStrip(suiseiseki, 5+xpad, 4, 12, 0, 190, 0);
setHStrip(suiseiseki, 3+xpad, 2, 13, 0, 190, 0);
setHStrip(suiseiseki, 9+xpad, 2, 13, 0, 190, 0);
setHStrip(suiseiseki, 2+xpad, 10, 14, 0, 190, 0);
setHStrip(suiseiseki, 4+xpad, 2, 8, 255, 255, 255);
setHStrip(suiseiseki, 8+xpad, 2, 8, 255, 255, 255);
setPixel(suiseiseki, 4+xpad, 9, 255, 255, 255);
setPixel(suiseiseki, 9+xpad, 9, 255, 255, 255);
setHStrip(suiseiseki, 6+xpad, 2, 9, 255, 255, 255);
setHStrip(suiseiseki, 2+xpad, 2, 10, 255, 255, 255);
setHStrip(suiseiseki, 10+xpad, 2, 10, 255, 255, 255);
setPixel(suiseiseki, 5+xpad, 10, 255, 255, 255);
setPixel(suiseiseki, 8+xpad, 10, 255, 255, 255);
setPixel(suiseiseki, 1+xpad, 11, 255, 255, 255);
setPixel(suiseiseki, 12+xpad, 11, 255, 255, 255);
setHStrip(suiseiseki, 6+xpad, 2, 8, 0, 0, 0);
setPixel(suiseiseki, 5+xpad, 9, 0, 0, 0);
setPixel(suiseiseki, 8+xpad, 9, 0, 0, 0);
setPixel(suiseiseki, 4+xpad, 10, 0, 0, 0);
setPixel(suiseiseki, 9+xpad, 10, 0, 0, 0);
setHStrip(suiseiseki, 6+xpad, 2, 10, 0, 0, 0);
setHStrip(suiseiseki, 5+xpad, 4, 11, 0, 0, 0);
setPixel(suiseiseki, 4+xpad, 12, 0, 0, 0);
setPixel(suiseiseki, 9+xpad, 12, 0, 0, 0);
setHStrip(suiseiseki, 5+xpad, 4, 13, 0, 0, 0);
setHStrip(suiseiseki, 1+xpad, 12, 15, 0, 0, 0);


--MESSING AROUND TEXTURES
quant3 = newTexture(16, 16);
copyTexture(quant3, suiseiseki);
quantize(quant3, 3, 3, 3);

quant5 = newTexture(16, 16);
copyTexture(quant5, suiseiseki);
quantize(quant5, 5, 5, 5);

hm = newTexture(16, 16);
copyTexture(hm, suiseiseki);
makeHeightMap(hm);
tint(hm, 0, 255, 60);

--fill(suiseiseki, 4, 0, 255, 0, 0);
--fill(suiseiseki, 6, 2, 0, 0, 255);
--fill(suiseiseki, 0, 0, 255, 0, 0);

--makeHeightMap(suiseiseki);
--tint(suiseiseki, 190, 80, 0);

--applyScale(suiseiseki, 1.0, 1.0, 255, 0, 0);
--applyScale(suiseiseki, 1.3, 1.3, 255, 0, 0);
--applyRotationAbout(suiseiseki, 900, 8, 8, 255, 0, 0);

makeFinal(suiseiseki);



--pixelFit(suiseiseki, .55, 0, 3000000, 0, 190, 0);

--test = newTexture(16, 16);
--setWithProbability(test, 265, .028906, 255, 255, 255);
--setWithProbability(test, 671039,.043359, 255, 255, 255);
--setWithProbability(test, 658412,.043359, 255, 255, 255);
--setWithProbability(test, 13537, .050781, 0, 190, 0);
--setWithProbability(test, 2250620, .055859, 0, 190, 0);
--makeFinal(test);
end