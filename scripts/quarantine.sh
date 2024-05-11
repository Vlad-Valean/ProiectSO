if [ $# -lt 2 ]; then
    echo "Usage: $0 <destination_directory> <file>"
    exit 1
fi

destination_directory="$1"
file="$2"

# Check if the destination directory exists and is a directory
if [ ! -d "$destination_directory" ]; then
    # echo "Error: '$destination_directory' is not a valid directory"
    exit 1
fi

# Check if the file exists
if [ ! -f "$file" ]; then
    # echo "Error: '$file' does not exist or is not a regular file"
    exit 1
fi

# Check if the file contains any keywords
if grep -Eq '(corrupted|dangerous|risk|attack|malware|malicious)' "$file"; then
    echo "$file"
    # Move the file to the destination directory
    mv "$file" "$destination_directory/"
fi