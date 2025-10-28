import os
import shutil

# Root folder containing all subfolders with CNF files
source_root = r"D:\Projects\Logic_Assignment\sc2002benchs"

# Destination folder where you want all CNF files collected
destination = r"D:\Projects\Logic_Assignment\cnffiles"

# Create destination folder if it doesn't exist
os.makedirs(destination, exist_ok=True)

# Walk through all subdirectories
for root, dirs, files in os.walk(source_root):
    for file in files:
        if file.lower().endswith(".cnf") or file.lower().endswith(".cnf.xz"):  # only CNF files
            source_path = os.path.join(root, file)
            destination_path = os.path.join(destination, file)

            # Handle duplicate filenames by renaming
            if os.path.exists(destination_path):
                base, ext = os.path.splitext(file)
                count = 1
                while os.path.exists(destination_path):
                    new_name = f"{base}_{count}{ext}"
                    destination_path = os.path.join(destination, new_name)
                    count += 1

            # Copy file (change to shutil.move if you want to move instead)
            shutil.copy2(source_path, destination_path)

print("All CNF files collected successfully!")