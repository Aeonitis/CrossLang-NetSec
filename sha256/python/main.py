import sys
import hashlib  # use hashlib instead of sha256 module

def read_file(file_path):
    try:
        with open(file_path, 'rb') as file:
            return file.read()
    except FileNotFoundError:
        print(f"Error: File not found - {file_path}")
        sys.exit(1)

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <file_path>")
    sys.exit(1)

file_path = sys.argv[1]
file_content = read_file(file_path)
hash = hashlib.sha256(file_content).hexdigest()[:8]  # Truncate hash to first 8 characters
print(hash)
