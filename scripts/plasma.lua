--Plasma
--by: Explorer

function setup()
xs = 256;
ys = 256;
handle = newTexture(xs, ys);
fractionalNoise(handle, -1, 0, 0, 0, 255, 255, 255);
temp = newTexture(xs, ys);
out = newTexture(xs, ys);

for i = 0,math.random(3, 6),1 do
 bt = BufferedTexture(handle);
 --bt = RepeatX(bt, xs);
 --bt = RepeatY(bt, ys);
 bt = InterpolateLinear(bt);
 bt = ScaleAbout(bt, 4*i, 4*i, xs/2, xs/2);
 --bt = RotateAbout(bt, math.random(20, 40), math.random((xs/2)-(xs/15), (xs/2)+(xs/15)), math.random((ys/2)-(ys/15), (ys/2)+(ys/15)));

 --bt = RotateAbout(bt, 30, 128, 128);
 RenderBack(temp, bt);
 add(out, temp, 1);
end

tint(out, math.random(150, 255), math.random(150, 255), math.random(150, 255));

makeFinal(out);

end