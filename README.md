\# 🔒 ADStream – NTFS Alternate Data Streams Manipulation



!\[C](https://img.shields.io/badge/Language-C-blue?style=flat-square\&logo=c)

!\[Windows](https://img.shields.io/badge/Platform-Windows-0078D4?style=flat-square\&logo=windows)

!\[NTFS](https://img.shields.io/badge/FileSystem-NTFS-green?style=flat-square)

!\[License](https://img.shields.io/badge/License-MIT-green?style=flat-square)



A lightweight Windows utility demonstrating \*\*Alternate Data Streams (ADS)\*\* manipulation in NTFS. Write hidden data to files, enumerate streams, and understand how attackers hide malicious payloads. \*\*Educational tool for forensic awareness and Windows internals research.\*\*



\---



\## 🎯 What are Alternate Data Streams?



\*\*Alternate Data Streams (ADS)\*\* is an NTFS feature that allows files to have multiple data streams beyond the default `:$DATA` stream.



\### Real-World Example

```

📄 document.pdf (visible, 2 MB)

&#x20;  ├── :$DATA (main stream - what you see)

&#x20;  └── :malware (hidden ADS - invisible in explorer!)

```



Users see only the PDF. The hidden ADS stream contains an executable that runs when the file is opened.



\---



\## ⚠️ Why This Matters



\### Offensive Use Cases

\- 🕵️ \*\*Hide malware payloads\*\* — Store executables in hidden streams

\- 📦 \*\*Data exfiltration\*\* — Hide stolen data in legitimate files

\- 🎭 \*\*Supply chain attacks\*\* — Inject backdoors into trusted files

\- 🚪 \*\*Living off the land\*\* — Use built-in NTFS features instead of dropping files



\### Defensive Implications

\- 🔍 \*\*Blind spots in AV\*\* — Many antivirus tools scan only main streams

\- 📊 \*\*Incomplete forensics\*\* — Investigators may miss hidden data

\- 🛡️ \*\*Persistence mechanism\*\* — Difficult to detect without proper tools

\- ⚡ \*\*Fast execution\*\* — No file creation = no file system artifacts



\---



\## ✨ Features



\- 📝 \*\*Write to ADS\*\* — Create hidden data streams on any file

\- 🔍 \*\*Enumerate Streams\*\* — List all streams (visible + hidden) using Windows APIs

\- 📖 \*\*Read ADS Data\*\* — Retrieve contents from hidden streams

\- ⚙️ \*\*API-based\*\* — Uses native Windows APIs (CreateFileW, FindFirstStreamW)

\- 🎓 \*\*Educational\*\* — Learn NTFS internals and ADS mechanics

\- 🔒 \*\*Forensic-Aware\*\* — Demonstrates why ADS detection is critical



\---



\## 🚀 Quick Start



\### Prerequisites

\- Windows 7+ (Vista+ for full ADS support)

\- NTFS file system

\- Visual Studio or MinGW (gcc/clang)

\- Basic C knowledge



\### Compilation



\*\*Visual Studio (MSVC):\*\*

```bash

cl adstream.c

```



\*\*MinGW (GCC):\*\*

```bash

gcc -o adstream.exe adstream.c

```



\### Usage



```bash

adstream.exe

```



\*\*What it does:\*\*

1\. ✅ Creates `test.txt` with main content "1"

2\. ✅ Creates an ADS named `:ads` with data "SECRET SAUCE"

3\. ✅ Enumerates all streams (main + hidden)

4\. ✅ Reads and displays hidden stream content



\*\*Output:\*\*

```

ADS inside:

(4)     ::$DATA

(13)    :ads:$DATA

\[+] Data from ADS: (13) SECRET SAUCE

```



\---



\## 📖 How It Works



\### Step-by-Step Flow



```

CREATE FILE

&#x20;   ↓

test.txt (4 bytes) → "1"

&#x20;   ↓

CREATE ADS STREAM

&#x20;   ↓

test.txt:ads (13 bytes) → "SECRET SAUCE"

&#x20;   ↓

ENUMERATE STREAMS

&#x20;   ↓

FindFirstStreamW() / FindNextStreamW()

&#x20;   ↓

LIST RESULTS

&#x20;   ├── (4) ::$DATA (main stream)

&#x20;   └── (13) :ads:$DATA (hidden stream)

&#x20;   ↓

READ ADS CONTENT

&#x20;   ↓

ReadFile(test.txt:ads)

&#x20;   ↓

OUTPUT: "SECRET SAUCE"

```



\---



\## 🔧 Technical Deep Dive



\### Creating an ADS



```c

// Open file with ADS specification

WCHAR\* target\_stream = L"D:\\\\test\\\\test.txt:hidden";



HANDLE tFile = CreateFileW(

&#x20;   target\_stream,

&#x20;   GENERIC\_READ | GENERIC\_WRITE,

&#x20;   FILE\_SHARE\_READ,

&#x20;   NULL,

&#x20;   OPEN\_ALWAYS,  // Create if doesn't exist

&#x20;   FILE\_ATTRIBUTE\_NORMAL,

&#x20;   NULL

);



// Write data to the ADS

BYTE data\[] = "SECRET DATA";

DWORD bytesWritten;

WriteFile(tFile, data, sizeof(data), \&bytesWritten, NULL);

```



\*\*Key Point:\*\* The `:hidden` suffix tells Windows to create an ADS named "hidden" instead of the main stream.



\### Enumerating Streams



```c

WIN32\_FIND\_STREAM\_DATA stream\_data;



// Find first stream

HANDLE hFind = FindFirstStreamW(

&#x20;   L"test.txt",

&#x20;   FindStreamInfoStandard,

&#x20;   \&stream\_data,

&#x20;   0

);



// Iterate through all streams

while (TRUE) {

&#x20;   printf("Stream: %S, Size: %I64u\\n", 

&#x20;          stream\_data.cStreamName,

&#x20;          stream\_data.StreamSize.QuadPart);

&#x20;   

&#x20;   if (!FindNextStreamW(hFind, \&stream\_data))

&#x20;       break;

}

```



\### Reading from ADS



```c

// Open the ADS for reading

HANDLE tFile = CreateFileW(

&#x20;   L"test.txt:hidden",

&#x20;   GENERIC\_READ,

&#x20;   FILE\_SHARE\_READ,

&#x20;   NULL,

&#x20;   OPEN\_EXISTING,

&#x20;   FILE\_ATTRIBUTE\_NORMAL,

&#x20;   NULL

);



// Read data

BYTE buffer\[1024];

DWORD bytesRead;

ReadFile(tFile, buffer, sizeof(buffer), \&bytesRead, NULL);



printf("ADS Content: %s\\n", (char\*)buffer);

```



\---



\## 📊 Lab Results



\### File Explorer View

```

test.txt

├── Size: 4 bytes (only shows main stream!)

├── Created: 07/16/2026 07:04 PM

└── Modified: 07/16/2026 07:05 PM

```



\### PowerShell View (with ADS detection)

```powershell

PS> Get-ChildItem -Stream \*



FileName     : test.txt

StreamSize   : 4

StreamName   : ::$DATA



FileName     : test.txt

StreamSize   : 13

StreamName   : :ads:$DATA

```



\### Command Prompt (dir /r)

```cmd

D:\\test\\alternate\_data\_stream>dir /r



07/16/2026  07:04 PM                 4 test.txt

&#x20;                                   13 test.txt:ads:$DATA

```



\---



\## 🛡️ Forensic \& Defense Implications



\### Why ADS Is Dangerous



1\. \*\*Invisible in Explorer\*\* ❌

&#x20;  - Windows Explorer shows only main stream size

&#x20;  - Hidden streams appear as "normal" files



2\. \*\*Bypasses File Scanning\*\* ❌

&#x20;  ```

&#x20;  File: document.pdf (scanned - clean)

&#x20;  ADS: :malware (not scanned - infected!)

&#x20;  ```



3\. \*\*Execution from ADS\*\* ⚡

&#x20;  ```cmd

&#x20;  REM Execute hidden executable from ADS

&#x20;  wmic process call create "C:\\file.txt:malware.exe"

&#x20;  ```



4\. \*\*Living off the Land\*\* 🎭

&#x20;  - No new files created (ADS is internal)

&#x20;  - No unusual process execution

&#x20;  - Looks like normal file activity



\### Detection Methods



\*\*1. Command Prompt\*\*

```cmd

dir /r  REM Shows ADS with sizes

```



\*\*2. PowerShell\*\*

```powershell

Get-Item file.txt -Stream \*

Get-Content file.txt -Stream streamname

```



\*\*3. Sysinternals\*\*

```cmd

streams.exe C:\\

```



\*\*4. Forensic Tools\*\*

\- EnCase (built-in ADS detection)

\- FTK Imager

\- Volatility

\- X-Ways Forensics



\*\*5. EDR/XDR\*\*

\- Monitor file open patterns with ADS syntax

\- Alert on `:` in file paths

\- Track WriteFile calls with stream specification



\---



\## 🎓 Learning Outcomes



By studying this code, you'll understand:



✅ \*\*NTFS Internals\*\*

\- Stream structures

\- File naming conventions with colons

\- Multi-stream file architecture



✅ \*\*Windows APIs\*\*

\- CreateFileW with stream syntax

\- FindFirstStreamW / FindNextStreamW

\- File enumeration at the stream level



✅ \*\*Evasion Techniques\*\*

\- Payload hiding mechanisms

\- Anti-forensics basics

\- Detection evasion strategies



✅ \*\*Forensics\*\*

\- How to detect ADS

\- What to look for during investigations

\- Complete forensic picture requires ADS analysis



\---



\## 📚 Resources



\*\*Windows API Documentation:\*\*

\- \[FindFirstStreamW](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findfirststream)

\- \[CreateFileW](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew)

\- \[NTFS Streams](https://docs.microsoft.com/en-us/openspecs/windows\_protocols/ms-fscc/c6e988d7-3771-4b86-a6c5-d5b71c4225f9)



\*\*Forensics \& Detection:\*\*

\- \[SANS: ADS Forensics](https://www.sans.org/reading-room/)

\- \[Alternate Data Streams Detection](https://www.pentestpartners.com/)

\- \[Windows NTFS Analysis](https://www.13cubed.com/)



\*\*Security Research:\*\*

\- \[Living Off The Land Binaries](https://lolbas-project.github.io/)

\- \[ATT\&CK: Hide Artifacts: NTFS File Attributes](https://attack.mitre.org/techniques/T1564/004/)



\---



\## 💡 Real-World Scenarios



\### Scenario 1: Supply Chain Attack

```

Attacker injects malware into legitimate PDF:

📄 whitepaper.pdf (2.5 MB) — signed, trusted

&#x20;  └── :payload.exe (5 MB) — hidden backdoor



When user opens PDF:

→ PDF reader opens main stream (safe)

→ Hidden malware executes silently

→ No new file created (no forensic footprint)

```



\### Scenario 2: Data Exfiltration

```

Attacker hides stolen data in common file:

📄 report.docx (100 KB) — normal, innocent

&#x20;  └── :stolen\_passwords.txt (50 KB) — hidden data



File passes inspection (looks normal)

Hidden stream contains database dumps

```



\### Scenario 3: Persistence

```

Attacker creates hidden startup script:

📄 C:\\Windows\\System32\\config\\SAM (visible)

&#x20;  └── :backdoor.cmd (hidden)



On next boot: Hidden script executes

No new task scheduler entries

No new startup folders

```



\---



\## ⚖️ Legal \& Ethical Note



This tool is for \*\*educational and authorized security research only\*\*.



Using ADS to hide malware or unauthorized code:

\- 🛑 Violates Computer Fraud \& Abuse Act

\- 📜 May constitute unauthorized access

\- 🔒 Violates organizational security policies



\*\*Always get explicit authorization before:\*\*

\- Creating ADS on systems you don't own

\- Hiding code or data

\- Testing evasion techniques



\---



\## 🔑 Key Takeaways



1\. \*\*ADS is real\*\* — NTFS supports multiple data streams on single files

2\. \*\*Invisible by default\*\* — Most tools show only main stream

3\. \*\*Powerful for evasion\*\* — No file creation = minimal forensic artifacts

4\. \*\*Detectable\*\* — With proper tools and investigation techniques

5\. \*\*Critical for forensics\*\* — Complete analysis requires ADS enumeration



\---



\## 📦 File Structure



```

adstream/

├── adstream.c              # Main implementation

├── README.md               # This file

├── NOTES.md                # Lab notes \& walkthrough

└── examples/

&#x20;   ├── basic\_ads.md        # Simple ADS creation

&#x20;   ├── malware\_hiding.md   # Evasion scenarios

&#x20;   └── forensic\_detection.md # How to find hidden streams

```



\---



\## 🛠️ Compilation Flags



\*\*With debugging symbols:\*\*

```bash

gcc -g -o adstream.exe adstream.c

```



\*\*Optimized build:\*\*

```bash

gcc -O2 -o adstream.exe adstream.c

```



\*\*With detailed warnings:\*\*

```bash

gcc -Wall -Wextra -o adstream.exe adstream.c

```



\---



\## 🤝 Contributing



Found a bug or want to improve this? Contributions welcome!



1\. Fork the repo

2\. Create a feature branch

3\. Make your changes

4\. Submit a Pull Request



\---



\## 📝 License



MIT License — See LICENSE file for details.



\---



\## 👨‍💻 Author



\*\*Zain\*\* — Cybersecurity researcher \& Windows internals enthusiast  

\[GitHub](https://github.com/zainsial866) 

\---



\## 🎯 Related Projects



\- \*\*\[Timestomper](https://github.com/zainsial866/timestomper)\*\* — File timestamp manipulation

\- \*\*\[Windows Stealth Primitives](https://github.com/zainsial866)\*\* — Advanced process injection techniques

\- \*\*\[Sentinel SOC Dashboard](https://github.com/zainsial866)\*\* — Security operations center monitoring



\---



<div align="center">



\*\*⭐ If this helps with your security research, consider starring the repo!\*\*



\*Made with ❤️ for the security community\*



</div>

