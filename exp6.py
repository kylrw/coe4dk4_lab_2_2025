import csv
import re

input_file = "exp6_output.txt"
output_file = "exp6_output.csv"

# Regex to match lines that are only numeric (with scientific notation etc.)
numeric_pattern = re.compile(r'^[\d.,]+$')

rows = []

rows.append(['Seed', 'Data_Arrival_Rate', 'Voice_Arrival_Rate', 'Mean_Delay', 'Voice_Mean_Delay', 'Data_Mean_Delay'])

# Try UTF-8 first, then fallback to others
for encoding in ('utf-8', 'utf-16', 'utf-16-le', 'utf-16-be', 'latin-1'):
    try:
        with open(input_file, 'r', encoding=encoding) as f:
            for line in f:
                stripped = line.strip()
                if stripped and numeric_pattern.match(stripped):
                    rows.append(stripped.split(','))
        print(f"✅ Successfully read file with encoding: {encoding}")
        break
    except UnicodeDecodeError:
        continue
else:
    raise ValueError(" Could not read file with any common encoding.")

# Write results
with open(output_file, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerows(rows)

print(f"Extracted {len(rows)} numeric lines to {output_file}")