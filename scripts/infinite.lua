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



--INFINITE TEXTURE FUN
bt = BufferedTexture(suiseiseki);  --casts suiseiseki to an infinite texture
bt = RepeatX(RepeatY(bt, 16), 16); --makes the texture infinitly repeat outside of 16, 16
bt = InterpolateLinear(bt);        --makes the texture linearly interpolate
bt = RotateAbout(bt, 45, 16, 16);  --rotates the testure 45 degrees about 16, 16
bt = Translate(bt, 64, 64);        --translates the texture by 64, 64
bt = ScaleAbout(bt, 2, 2, 16, 16); --scales the texture by a factor of 2 about 16, 16
out = newTexture(128, 128);        --creates a destination (out)
RenderBack(out, bt);               --renders the infinite texture back to a normal texture (out)

makeFinal(out);
end
