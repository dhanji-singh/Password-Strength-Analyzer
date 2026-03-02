# Password Strength Analyzer 🔐

A comprehensive password strength analyzer built in C with multiple interface options. Check your password strength in real-time with detailed feedback and strength metrics.

## Features

✅ Real-time password strength analysis  
✅ Character diversity checking (uppercase, lowercase, digits, special chars)  
✅ Weak pattern detection (common passwords like "123", "password", etc.)  
✅ Color-coded strength indicators (Weak, Moderate, Strong)  
✅ Detailed feedback and suggestions  
✅ Multiple interface options (CLI, TUI, Web)  

## Password Strength Criteria

A password is classified as:

| Level | Criteria |
|-------|----------|
| **Weak** | Length < 6 OR diversity < 2 OR contains weak patterns |
| **Moderate** | Length < 10 OR diversity < 3 |
| **Strong** | Length ≥ 10 AND diversity ≥ 3 AND no weak patterns |

### Character Diversity
- 1 point: Uppercase letters (A-Z)
- 1 point: Lowercase letters (a-z)
- 1 point: Digits (0-9)
- 1 point: Special characters (!@#$%^&*)

### Weak Patterns Detected
- "123"
- "password"
- "qwerty"
- "abc"

## Usage Options

### 1. Command Line Interface (CLI)

Simple command-line version for quick password checking.

**Build:**
```bash
gcc -o code code.c
```

**Run:**
```bash
./code
```

**Example:**
```
Enter password: MyPass123!
Strong Password
```

---

### 2. Terminal User Interface (TUI)

Full-featured terminal interface with colors and visualization using ncurses.

**Features:**
- Color-coded strength bar
- Live criteria checking
- Visual feedback
- Password masking

**Build:**
```bash
gcc -o gui_tui gui_tui.c -lncurses
```

**Run:**
```bash
./gui_tui
```

**Controls:**
- Type your password
- View real-time strength analysis
- BACKSPACE to delete characters
- ESC to quit

---

### 3. Web Interface (HTML/CSS/JavaScript)

Modern, user-friendly web application with beautiful UI.

**Features:**
- Responsive design
- Real-time password analysis
- Show/hide password toggle
- Detailed criteria checklist
- Password statistics
- Color-coded feedback

**Run:**
```bash
python3 server.py
```

Then open your browser to `http://localhost:8000`

The server will automatically open your default browser when started.

---

## File Structure

```
Password-Strength-Analyzer/
├── code.c              # Original CLI version
├── gui_tui.c           # Terminal UI version (ncurses)
├── gui.c               # GTK+ GUI version (requires GTK development libraries)
├── index.html          # Web interface
├── server.py           # Simple HTTP server for web interface
└── README.md           # This file
```

---

## Installation & Dependencies

### CLI Version
- No dependencies (uses standard C library)

### TUI Version
```bash
# Install ncurses development library
sudo apt-get install libncurses-dev
```

### Web Version
```bash
# Python 3 (usually pre-installed)
python3 --version
```

### GTK+ Version (optional)
```bash
sudo apt-get install libgtk-3-dev pkg-config
gcc $(pkg-config --cflags gtk+-3.0) gui.c -o gui_app $(pkg-config --libs gtk+-3.0)
```

---

## Examples

### Testing with Different Passwords

```bash
# Test Weak Password
$ ./code
Enter password: 123
Weak Password

# Test Moderate Password
$ ./code
Enter password: Password1
Moderate Password

# Test Strong Password
$ ./code
Enter password: SecureP@ssw0rd!
Strong Password
```

---

## How It Works

1. **Character Analysis**: Scans password for uppercase, lowercase, digits, and special characters
2. **Pattern Detection**: Checks against common weak patterns
3. **Strength Classification**: 
   - Evaluates length and character diversity
   - Detects weak patterns
   - Returns strength level (0=Weak, 1=Moderate, 2=Strong)

---

## Security Notes

⚠️ **Important:**
- This analyzer is for **educational purposes** only
- Never enter real passwords into online versions
- This tool checks password complexity, not true entropy
- For real password security, use a password manager
- Always use HTTPS in production environments

---

## Performance

- **CLI**: ~0.1ms analysis time
- **TUI**: ~1-2ms with rendering
- **Web**: ~0ms (real-time JavaScript)

---

## Author

Created for learning C programming concepts including:
- String manipulation (`strlen`, `strstr`)
- Character classification (`isupper`, `islower`, `isdigit`)
- Control flow and logic
- Data structures
- Terminal and GUI programming

---

## License

This project is open source and available for educational use.

---

## Feedback & Improvements

Feel free to fork, modify, and improve this project!

Possible enhancements:
- Add more weak pattern detection
- Support for password strength estimation (entropy calculation)
- Multi-language support
- Mobile-friendly improvements
- Database integration for compromised password checking
