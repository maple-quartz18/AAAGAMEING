
import pefile
pe = pefile.PE("/Volumes/LexarSSD/Bottles/11/drive_c/GTA6XBOXS/game_scarlett_master.exe") 
for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
    if exp.name:
        print(exp.name.decode())
' | sort

