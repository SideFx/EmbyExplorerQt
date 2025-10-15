Emby Explorer (Qt)

Browse and export Emby collections

Welcome screen
![grafik](https://github.com/user-attachments/assets/2911aa53-ae0b-4dc5-ba99-0b2ed9036c5f)


Fetched Home videos collection
![grafik](https://github.com/user-attachments/assets/17e72145-0418-4e29-b110-07c9e81cd099)


Collection exported to Excel
![grafik](https://github.com/user-attachments/assets/de847d2c-6c7c-4353-b118-0bc38425479d)

Credits go to:
Daniel Nicoletti (dantti) and Jay Two (j2Doll) for "QXlsx": https://github.com/QtExcel/QXlsx


On MacOS the app needs network access granted. (First logon to Emby server may fail.)

19.September 2025:
- Fix for broken ToolBar->ToolButton style on MacOS (no hover effect, no click effect)
- Added ".json" extension to pref file
- Added qbase_de.qm file (translate MacOS system menu)
- minor enhancements

14/15. October 2025:
- all releases built with Qt6.10
- code review
- preferences now use ascii85 format (instead of base64)
- CMakeList: copy qm files to target/translations (Windows)
- minor changes
  
Have fun.





















