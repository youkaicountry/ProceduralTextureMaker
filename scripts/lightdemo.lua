function setup()

--setup bump
bump1 = newTexture(128, 128);
filledRect(bump1, 30, 30, 60, 60, 200, 200, 200);
applyBlur(bump1, 0, 5, 1, 1, 1);
--setup pic
pic1 = newTexture(128, 128);
clearTexture(pic1, 140, 20, 20);
directionLight(pic1, bump1, 20, 3, 0, -1, 1, 1, 0);
--quantize(pic1, 5, 5, 5);
makeFinal(pic1);

--setup bump2
bump2 = newTexture(256, 256);
perlinNoise(bump2, 10, 10, 30, .6, 3, 12);
pic2 = newTexture(256, 256);
clearTexture(pic2, 140, 140, 140);
light(pic2, bump2, 22, 128, 128, 40, .5, .75, 1, 0, 0);
makeFinal(pic2);

--wood
pn1 = newTexture(256, 256);
perlinNoise(pn1, 1, 1, 50.0, .55, 4, 88);
applyScale(pn1, 1, 7, 0, 0, 0);
tint(pn1, 190, 130, 0);
bump3 = newTexture(256, 256);
copyTexture(bump3, pn1);
makeHeightMap(bump3);
applyBlur(bump3, 0, 3, 1, 1, 1);
light(pn1, bump3, 40, 30, 30, 50, 1, 2, 1, 0, 0);
makeFinal(pn1);

--circles
circbump = newTexture(128, 128);
filledCircle(circbump, 64, 64, 50, 50, 50, 50);
filledCircle(circbump, 64, 64, 30, 100, 100, 100);
filledCircle(circbump, 64, 64, 10, 150, 150, 150);
circ = newTexture(128, 128);
clearTexture(circ, 30, 200, 40);
applyBlur(circbump, 0, 2, 1, 1, 1);
applyGaussianBlur(circbump, 0, 5, 1, 1, 1);
light(circ, circbump, 40, 0, 0, 46, .5, .5, 1, 0, 0);
makeFinal(circ);
end