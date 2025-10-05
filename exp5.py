import csv
import re

input_file = "exp5_output.txt"
output_file = "exp5_output.csv"

# Regex to match lines that are only numeric (with scientific notation etc.)
numeric_pattern = re.compile(r'^[\d.\sEe+-]+$')

rows = []

rows.append(['p12', 'mean_ms_S1', 'mean_ms_S2', 'mean_ms_S3', 'amount_S1', 'amount_S2', 'amount_S3'])

# Try UTF-8 first, then fallback to others
for encoding in ('utf-8', 'utf-16', 'utf-16-le', 'utf-16-be', 'latin-1'):
    try:
        with open(input_file, 'r', encoding=encoding) as f:
            for line in f:
                stripped = line.strip()
                stripped = re.sub(r'\s+', ' ', stripped)
                if stripped and numeric_pattern.match(stripped):
                    rows.append(stripped.split())
        print(f"✅ Successfully read file with encoding: {encoding}")
        break
    except UnicodeDecodeError:
        continue
else:
    raise ValueError("❌ Could not read file with any common encoding.")

# Write results
with open(output_file, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerows(rows)

print(f"Extracted {len(rows)} numeric lines to {output_file}")
