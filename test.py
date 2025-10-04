import csv
import random

# CSV settings
filename = "file.csv"
headers = [f"Col{i}" for i in range(101)]  # Col0 ... Col100
num_rows = 10000  # number of rows you want

# Open CSV and write
with open(filename, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(headers)  # write headers
    
    for _ in range(num_rows):
        # generate 101 random values between 1 and 100
        row = [random.uniform(1, 100) for _ in range(101)]
        writer.writerow(row)

print(f"{filename} created with {num_rows} rows and {len(headers)} columns ✅")

