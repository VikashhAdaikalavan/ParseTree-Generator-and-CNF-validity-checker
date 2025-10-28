import os
import lzma

# Folder where your .cnf.xz files are collected
source_folder = r"D:\Projects\Logic_Assignment\cnffiles"

# (Optional) Folder to save the extracted .cnf files
# If you want them in the same folder, set destination_folder = source_folder
destination_folder = r"D:\Projects\Logic_Assignment\cnfextractedfiles"
os.makedirs(destination_folder, exist_ok=True)

for file in os.listdir(source_folder):
    if file.lower().endswith(".cnf.xz"):
        xz_path = os.path.join(source_folder, file)
        cnf_filename = file[:-3]  # remove '.xz' at the end
        cnf_path = os.path.join(destination_folder, cnf_filename)

        try:
            with lzma.open(xz_path, "rb") as compressed:
                with open(cnf_path, "wb") as output:
                    output.write(compressed.read())
            print(f"Extracted: {file} → {cnf_filename}")
        except Exception as e:
            print(f"Error extracting {file}: {e}")

print("All .cnf.xz files extracted successfully!")
