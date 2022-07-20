![K9 Web Protection Password Cracker Logo](k9wppc_logo.png)

# K9 Web Protection Password Cracker

This tool helps in the password recovery of K9 Web Protection installations and is based on [the weak hashing algorithm](Password-Algorithms-K9-Web-Protection-Admin-Insecurety-Research.pdf) discovered by "dietrich" originally posted on insecurety.net (now removed). The program goes through the "license" file to offset 0x2c and begins subtracting 0x50 from each byte until an invalid character is produced. At testing, this process was fairly successful but not perfect, often producing partial passwords or adding extra characters.

This project is not actively maintained and was last updated in 2014. Feel free to fork and improve this tool.

## Download

You can [download the Windows executable](K9%20Web%20Protection%20Password%20Cracker.zip) (compiled in 2014) or attempt to fork and compile [the source code](main.cpp).

## Screenshots

![K9WPPC Screenshot](screenshots/K9%20Web%20Protection%20Password%20Cracker_s1.jpg)
![K9WPPC Screenshot](screenshots/K9%20Web%20Protection%20Password%20Cracker_s2.jpg)
![K9WPPC Screenshot](screenshots/K9%20Web%20Protection%20Password%20Cracker_s3.jpg)
