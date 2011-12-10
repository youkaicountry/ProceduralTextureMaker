--Mossy Rock
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

--bumpmap moss
mossbump = newTexture(256, 256);
copyTexture(terrain2, terrain);
tint(terrain2, 200, 200, 200);
applyBlur(terrain2, 0, 8, 1, 1, 1);
quantize(terrain2, 2, 2, 2);
applyBlur(terrain2, 0, 1, 1, 1, 1);
copyTexture(mossbump, terrain2);
applyBlur(mossbump, 0, 4, 1, 1, 1);

--combine moss
moss = newTexture(256, 256);
tint(terrain2, 127, 192, 0);
copyTexture(moss, terrain);
add(moss, terrain2, 5);
light(moss, mossbump, 8.0, 128, 128, 128, 0.0, 1.0, 1.0, 0.0, 0);
applyRotationAbout(moss, 90.0, 128, 128, 0, 0, 0);

--combine
copyTexture(terrain2, terrain);
tint(terrain, 112, 96, 96);
light(terrain, terrain2, 1.0, 128, 128, 12, 0.0, 1.0, 1.0, -0.3, 0);
overlay(terrain, moss, 0.0, 0, 0, 0);
makeFinal(terrain);

end;