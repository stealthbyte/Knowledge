import ctypes

# DWORD WINAPI FormatMessage(
#   __in      DWORD dwFlags,
#   __in_opt  LPCVOID lpSource,
#   __in      DWORD dwMessageId,
#   __in      DWORD dwLanguageId,
#   __out     LPTSTR lpBuffer,
#   __in      DWORD nSize,
#   __in_opt  va_list *Arguments
# );


def valid_check(result):
    if result == 0:
        raise ctypes.WinError()

    return result


FormatMessage = ctypes.windll.kernel32.FormatMessageA
# FORMAT_MESSAGE_FROM_SYSTEM
dwFlags = 0x00001000
lpSource = 0
dwMessageId = int(input("Enter the Error code: "))
dwLanguageId = 0
lpBuffer = ctypes.create_string_buffer(255)
nSize = ctypes.sizeof(lpBuffer)
args = 0

FormatMessage.restype = valid_check
FormatMessage(dwFlags,
              lpSource,
              dwMessageId,
              dwLanguageId,
              lpBuffer,
              nSize,
              args)

print(lpBuffer.value.decode())
