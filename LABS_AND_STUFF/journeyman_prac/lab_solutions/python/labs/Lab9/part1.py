import ctypes
# int WINAPI MessageBox(
#   _In_opt_  HWND hWnd,
#   _In_opt_  LPCTSTR lpText,
#   _In_opt_  LPCTSTR lpCaption,
#   _In_      UINT uType
# );

# Constants
MB_SETFOREGROUND = 0x00010000
IDYES = 6
MB_YESNO = 4
MB_ICONERROR = 0x10
MB_ICONQUESTION = 0x20

MessageBox = ctypes.windll.user32.MessageBoxA

ret_val = MessageBox(
    0,
    b"Isn't Python Awesome?",
    b"You Know The Answer",
    MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND
)

while IDYES != ret_val:
    MessageBox(
        0,
        b"WRONG!",
        b"Incorrect",
        MB_ICONERROR | MB_SETFOREGROUND
    )

    ret_val = MessageBox(
        0,
        b"Isn't Python Awesome?",
        b"You Know The Answer",
        MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND
    )
