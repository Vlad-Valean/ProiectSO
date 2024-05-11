if [ $# -lt 2 ]; then
    echo "Usage: $0 <destination_directory> <file>"
    exit 1
fi

destination_directory="$1"
file="$2"
chars=$(wc -c < "$file")
words=$(wc -w < "$file")
lines=$(wc -l < "$file")


if [ ! -d "$destination_directory" ]; then
    echo "Error: '$destination_directory' is not a valid directory"
    exit 1
fi

if [ ! -f "$file" ]; then
    echo "Error: '$file' does not exist or is not a regular file"
    exit 1
fi

if grep -qP "[^\x00-\x7F]" "$file" ||
    grep -Eq '(corrupted|dangerous|risk|attack|malware|malicious)' "$file" || 
    ([ "$chars" -gt 2000 ] && [ "$words" -gt 1000 ] && [ "$lines" -lt 3 ]); 
    then
        echo "$file"
        mv "$file" "$destination_directory/"
else
    echo "SAFE"
fi