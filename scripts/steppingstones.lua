--Stepping Stones
--by Cryoburner

function setup()

--texture
terrain = newTexture(256, 256);
terrain2 = newTexture(256, 256);
fractionalNoise(terrain2, 0, os.time(), 0, 0, 0, 64, 64, 64);
copyTexture(terrain2, terrain);
for i = 1, 4, 1 do
 applyScale(terrain, 2.0, 2.0, 0, 0, 0)
 applyBlur(terrain, 0, 1, 1, 1, 1)
 add(terrain, terrain2, 2)
end;

--bumpmap
clearTexture(terrain2, 0, 0, 0);
for i = 1, 255, 1 do
 filledCircle(terrain2, math.random(255), math.random(255), math.random(31), i, i, i)
end;
for i = 1, 255, 1 do
 filledCircle(terrain2, math.random(255), math.random(255), math.random(2), 0, 0, 0)
end;
applyBlur(terrain2, 0, 1, 1, 1, 1);

--colormap
color = newTexture(256, 256);
copyTexture(color, terrain2);
tint(color, 160, 160, 160);
quantize(color, 2, 2, 2);
colourReplace(color, 1.0, 0, 0, 0, 0, 56, 64);
colourReplace(color, 1.0, 128, 128, 128, 128, 112, 96);
applyBlur(color, 0, 1, 1, 1, 1);

--combine
light(terrain, terrain2, 8.0, 128, 128, 96, 0.0, 0.75, 1.0, 0.0, 0);
add(terrain, color, 2);
makeFinal(terrain);

end;