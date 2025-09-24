import csv

# Data to write
headers = ["Col1", "Col2", "Col3", "Col4"]
rows = [
    [10.5, 20.1, 30.7, 40.0],
    [11.2, 21.5, 31.8, 41.3],
    [12.0, 22.4, 32.6, 42.9],
]

# Write CSV file
with open("file.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(headers)  # write header
    writer.writerows(rows)    # write data

print("file.csv created ✅")

