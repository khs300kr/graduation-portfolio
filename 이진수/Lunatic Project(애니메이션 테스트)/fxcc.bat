set PATH=c:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86\
cd TimeTraveller
fxc /Od /Zi /Ges /T vs_5_0 /E VSSkinned /Fo VS.fxo CharacterSkinned.fx
fxc /Od /Zi /Ges /T ps_5_0 /E PSSkinned /Fo PS.fxo CharacterSkinned.fx
cd ..