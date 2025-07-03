#include <errnum.h>

const char *errToString(error_t errnum)
{
    switch (errnum)
    {
    case ERR_INVALID_FUNCTION:
        return "Invalid function number";
    case ERR_FILE_NOT_FOUND:
        return "File not found";
    case ERR_PATH_NOT_FOUND:
        return "Path not found";
    case ERR_TOO_MANY_OPEN_FILES:
        return "Too many open files";
    case ERR_ACCESS_DENIED:
        return "Access denied";
    case ERR_INVALID_HANDLE:
        return "Invalid handle";
    case ERR_MCB_DESTROYED:
        return "Memory control blocks destroyed";
    case ERR_INSUFFICIENT_MEMORY:
        return "Insufficient memory";
    case ERR_INVALID_BLOCK_ADDRESS:
        return "Invalid memory block address";
    case ERR_INVALID_ENVIRONMENT:
        return "Invalid environment";
    case ERR_INVALID_FORMAT:
        return "Invalid format";
    case ERR_INVALID_ACCESS_MODE:
        return "Invalid access mode";
    case ERR_INVALID_DATA:
        return "Invalid data";
    case ERR_INVALID_DRIVE:
        return "Invalid drive specified";
    case ERR_REMOVE_CURRENT_DIRECTORY:
        return "Attempt to remove current directory";
    case ERR_NOT_SAME_DEVICE:
        return "Not same device";
    case ERR_NO_MORE_FILES:
        return "No more files";
    case ERR_WRITE_PROTECTED_DISKETTE:
        return "Attempt to write on write-protected diskette";
    case ERR_UNKNOWN_UNIT:
        return "Unknown unit";
    case ERR_DRIVE_NOT_READY:
        return "Drive not ready";
    case ERR_UNKNOWN_COMMAND:
        return "Unknown command";
    case ERR_CRC_ERROR:
        return "CRC error";
    case ERR_BAD_REQUEST_LENGTH:
        return "Bad request structure length";
    case ERR_SEEK_ERROR:
        return "Seek error";
    case ERR_UNKNOWN_MEDIA_TYPE:
        return "Unknown media type";
    case ERR_SECTOR_NOT_FOUND:
        return "Sector not found";
    case ERR_PRINTER_OUT_OF_PAPER:
        return "Printer out of paper";
    case ERR_WRITE_FAULT:
        return "Write fault";
    case ERR_READ_FAULT:
        return "Read fault";
    case ERR_GENERAL_FAILURE:
        return "General failure";
    case ERR_SHARING_VIOLATION:
        return "Sharing violation";
    case ERR_LOCK_VIOLATION:
        return "Lock violation";
    case ERR_INVALID_DISK_CHANGE:
        return "Invalid disk change";
    case ERR_FCB_UNAVAILABLE:
        return "FCB unavailable";
    case ERR_SHARING_BUFFER_OVERFLOW:
        return "Sharing buffer overflow";
    case ERR_UNABLE_TO_COMPLETE_OPERATION:
        return "Unable to complete file operation";
    case ERR_NETWORK_REQ_NOT_SUPPORTED:
        return "Network request not supported";
    case ERR_REMOTE_NOT_LISTENING:
        return "Remote computer not listening";
    case ERR_DUPLICATE_NAME:
        return "Duplicate name on network";
    case ERR_NETWORK_NAME_NOT_FOUND:
        return "Network name not found";
    case ERR_NETWORK_BUSY:
        return "Network busy";
    case ERR_DEVICE_NO_LONGER_EXISTS:
        return "Network device no longer exists";
    case ERR_NETBIOS_COMMAND_LIMIT:
        return "NetBIOS command limit exceeded";
    case ERR_NETWORK_ADAPTER_ERROR:
        return "Network adapter error";
    case ERR_INCORRECT_NETWORK_RESPONSE:
        return "Incorrect network response";
    case ERR_UNEXPECTED_NETWORK_ERROR:
        return "Unexpected network error";
    case ERR_INCOMPATIBLE_REMOTE_ADAPTER:
        return "Incompatible remote adapter";
    case ERR_PRINT_QUEUE_FULL:
        return "Print queue full";
    case ERR_NO_SPACE_FOR_PRINT_FILE:
        return "No space for print file";
    case ERR_PRINT_FILE_DELETED:
        return "Print file deleted";
    case ERR_NETWORK_NAME_DELETED:
        return "Network name deleted";
    case ERR_NETWORK_ACCESS_DENIED:
        return "Access denied";
    case ERR_NETWORK_DEVICE_TYPE_INCORRECT:
        return "Network device type incorrect";
    case ERR_NETWORK_NAME_NOT_FOUND2:
        return "Network name not found";
    case ERR_NETWORK_NAME_LIMIT_EXCEEDED:
        return "Network name limit exceeded";
    case ERR_NETBIOS_SESSION_LIMIT:
        return "NetBIOS session limit exceeded";
    case ERR_TEMPORARILY_PAUSED:
        return "Temporarily paused";
    case ERR_REQUEST_NOT_ACCEPTED:
        return "Network request not accepted";
    case ERR_REDIRECTION_PAUSED:
        return "Print or disk redirection paused";
    case ERR_FILE_ALREADY_EXISTS:
        return "File already exists";
    case ERR_CANNOT_MAKE_DIR_ENTRY:
        return "Cannot make directory entry";
    case ERR_FAIL_ON_INT24:
        return "Fail on INT 24";
    case ERR_TOO_MANY_REDIRECTIONS:
        return "Too many redirections";
    case ERR_DUPLICATE_REDIRECTION:
        return "Duplicate redirection";
    case ERR_INVALID_PASSWORD:
        return "Invalid password";
    case ERR_INVALID_PARAMETER:
        return "Invalid parameter";
    case ERR_NETWORK_DEVICE_FAULT:
        return "Network device fault";
    case ERR_NETWORK_FN_NOT_SUPPORTED:
        return "Function not supported by network";
    case ERR_COMPONENT_NOT_INSTALLED:
        return "Required system component not installed";
    case ERR_INVALID_ARGS:
        return "Invalid arguments passed to the function";
    default:
        return "Unknown DOS error";
    }

    return 0;
}

const char *actionToString(action_t action)
{
    switch (action)
    {
    case ACTION_NO_ACTION:
        return "No error; No action required";
    case ACTION_RETRY:
        return "Retry: attempt a few more times and re-prompt";
    case ACTION_DELAY_RETRY:
        return "Delay retry: pause, then retry a few more times";
    case ACTION_REENTER_INPUT:
        return "Re-enter input: prompt user to re-enter input";
    case ACTION_ABORT_WITH_CLEANUP:
        return "Abort with cleanup: orderly abort and shutdown";
    case ACTION_IMMEDIATE_ABORT:
        return "Immediate abort: exit immediately without cleanup";
    case ACTION_IGNORE:
        return "Ignore the error";
    case ACTION_USER_INTERVENTION:
        return "User intervention required: retry after user fixes the problem";
    case ACTION_FIX_ARGS:
        return "Fix the function arguments";
    default:
        return "Unknown or undefined error action";
    }

    return 0;
}