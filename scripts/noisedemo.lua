function setup()

--glass
glass = newTexture(128, 128);
setWithProbability(glass, 33, .001, 255, 255, 255);
g2 = newTexture(128, 128);
noise(g2, 33, 0, 0, 0, 255, 255, 255);
add(glass, g2, 5);
closestColour(glass, 1.0, 0, 0, 0);
--applyEmboss(glass, 0, 1, 1, 1, 1);
makeFinal(glass);

--gooble
gooble = newTexture(128, 128);
fractionalNoise(gooble, 18, 74, 100, 100, 200, 120, 120);
applyRotationAbout(gooble, 30, 64, 64, 255, 0, 0);
applyScale(gooble, 2.2, 2.2, 255, 0, 0);
applyGaussianBlur(gooble, 0, 3, 1, 1, 1);
makeFinal(gooble);

--scattered circle
scirc = newTexture(128, 128);
g3 = newTexture(128, 128);
setWithProbability(scirc, 125, .3, 255, 255, 255);
filledCircle(g3, 64, 64, 45, 255, 40, 40);
add(scirc, g3, 5);
applyBlur(scirc, 0, 1, 1, 1, 1);
makeFinal(scirc);

--limited colour noise
lcn = newTexture(128, 128);
noise(lcn, 202, 0, 0, 0, 255, 255, 255);
quantize(lcn, 2, 2, 1);
makeFinal(lcn);

--wood
pn1 = newTexture(256, 256);
perlinNoise(pn1, 1, 1, 50.0, .55, 4, 88);
applyScale(pn1, 1, 10, 0, 0, 0);
tint(pn1, 190, 130, 0);
--applyEmboss(pn1, 0, 1, 1, 1, 1);
makeFinal(pn1);

--quantperlin
qp = newTexture(128, 128);
perlinNoise(qp, 1, 1, 50, .55, 4, 70);
quantize(qp, 8, 8, 8);
applyEdgeDetectLaplace(qp, 0, 1, 1, 1, 1);
makeFinal(qp);

end