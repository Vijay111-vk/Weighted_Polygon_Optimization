#!/usr/bin/env python3
import subprocess
import os
import sys

def compile_executables():
    """
    Compile all required C++ files.
    If you already have the executables, you can comment out this function.
    """
    files_to_compile = [
        ("Knapsack.cpp", "Knapsack"),
        ("PolygonConstruction.cpp", "PolygonConstruction"),
        ("checker.cpp", "checker"),
        ("Top250.cpp", "Top250"),
        ("Annealing.cpp", "Annealing")  # Not used in the current chain.
    ]
    for src, exe in files_to_compile:
        print(f"Compiling {src} into {exe}...")
        result = subprocess.run(["g++", src, "-Wl,--stack,8388608", "-o", exe])
        if result.returncode != 0:
            print(f"Compilation failed for {src}. Exiting.")
            sys.exit(1)
    print("All files compiled successfully.\n")

def run_for_input(input_file, index):
    print(f"\n=== Processing {input_file} ===")
    
    # Define directories for intermediate and final output files.
    intermediate_dir = "intermediate"
    final_output_dir = "output"
    
    # Ensure the directories exist.
    os.makedirs(intermediate_dir, exist_ok=True)
    os.makedirs(final_output_dir, exist_ok=True)
    
    # Get the absolute path of the input file.
    abs_input_file = os.path.abspath(input_file)
    
    # --------------------------------------------------
    # 1. Run Knapsack.cpp:
    #    It reads from standard input and creates Boolean_B.txt in intermediate.
    try:
        with open(abs_input_file, "r") as f_in:
            # Run Knapsack without command-line arguments.
            result = subprocess.run(["./Knapsack.exe"], stdin=f_in, cwd=intermediate_dir)
            if result.returncode != 0:
                print("Error running Knapsack.")
                return 0
    except Exception as e:
        print(f"Error running Knapsack for {input_file}: {e}")
        return 0
    # --------------------------------------------------
    # 2. Run PolygonConstruction.cpp:
    #    It reads intermediate/Boolean_B.txt and creates Edges.txt.
    boolean_b_path = os.path.join(intermediate_dir, "Boolean_B.txt")
    if not os.path.exists(boolean_b_path):
        print("Boolean_B.txt not found. Skipping this input.")
        return 0
    try:
        with open(boolean_b_path, "r") as f_in:
            result = subprocess.run(["./PolygonConstruction.exe"], stdin=f_in, cwd=intermediate_dir)
            if result.returncode != 0:
                print("Error running PolygonConstruction.")
                return 0
    except Exception as e:
        print(f"Error running PolygonConstruction: {e}")
        return 0
    
    # --------------------------------------------------
    # 3. Run checker.cpp:
    #    It needs two inputs: Edges.txt and the original input.
    #    We'll create a combined file (checker_input.txt) by concatenating Edges.txt and the input file.
    edges_path = os.path.join(intermediate_dir, "Edges.txt")
    if not os.path.exists(edges_path):
        print("Edges.txt not found. Skipping this input.")
        return 0
    combined_checker_input = os.path.join(intermediate_dir, "checker_input.txt")
    try:
        with open(combined_checker_input, "w") as outfile:
            # Write the contents of Edges.txt.
            with open(edges_path, "r") as infile:
                outfile.write(infile.read())
            # Optionally, add a newline separator.
            outfile.write("\n")
            # Write the contents of the original input file.
            with open(abs_input_file, "r") as infile:
                outfile.write(infile.read())
    except Exception as e:
        print(f"Error creating combined checker input: {e}")
        return 0
    
    try:
        with open(combined_checker_input, "r") as f_in:
            result = subprocess.run(["./checker.exe"], stdin=f_in, cwd=intermediate_dir)
            # result = subprocess.run(["./checker", "./intermediate/Edges.txt", combined_checker_input] , cwd=intermediate_dir)
            if result.returncode != 0:
                print("Error running checker.")
                return 0
    except Exception as e:
        print(f"Error running checker: {e}")
        return 0

    # --------------------------------------------------


    # I want to store the output.txt content into new file outputKnapsack.txt
    output_txt_path_temp = os.path.join(intermediate_dir, "output.txt")
    outputKnapsack_txt_path = os.path.join(intermediate_dir, "outputKnapsack.txt")
    try:
        with open(output_txt_path_temp, "r") as f:
            output_contents = f.read().strip()
        with open(outputKnapsack_txt_path, "w") as f:
            f.write(output_contents)
    except Exception as e:
        print(f"Error processing output files: {e}")
        return 0
    
     
    # --------------------------------------------------
    # 1. Run Annealing.cpp:
    #    It reads from standard input and creates Boolean_B.txt in intermediate.
    try:
        with open(abs_input_file, "r") as f_in:
            # Run Knapsack without command-line arguments.
            result = subprocess.run(["./Annealing.exe"], stdin=f_in, cwd=intermediate_dir)
            if result.returncode != 0:
                print("Error running Annealing.")
                print(result.returncode) 
                return 0
    except Exception as e:
        print(f"Error running Knapsack for {input_file}: {e}")
        return 0
    # --------------------------------------------------
    # 2. Run PolygonConstruction.cpp:
    #    It reads intermediate/Boolean_B_Annealing.txt and creates Edges.txt.
    boolean_b_path = os.path.join(intermediate_dir, "Boolean_B_Annealing.txt")
    if not os.path.exists(boolean_b_path):
        print("Boolean_B_Annealing.txt not found. Skipping this input.")
        return 0
    try:
        with open(boolean_b_path, "r") as f_in:
            result = subprocess.run(["./PolygonConstruction.exe"], stdin=f_in, cwd=intermediate_dir)
            if result.returncode != 0:
                # print("Error running PolygonConstruction.")
                return 0
    except Exception as e:
        print(f"Error running PolygonConstruction: {e}")
        return 0
    
    # --------------------------------------------------
    # 3. Run checker.cpp:
    #    It needs two inputs: Edges.txt and the original input. 
    #    We'll create a combined file (checker_input.txt) by concatenating Edges.txt and the input file.
    edges_path = os.path.join(intermediate_dir, "Edges.txt")
    if not os.path.exists(edges_path):
        print("Edges.txt not found. Skipping this input.")
        return 0
    combined_checker_input = os.path.join(intermediate_dir, "checker_input.txt")
    try:
        with open(combined_checker_input, "w") as outfile:
            # Write the contents of Edges.txt.
            with open(edges_path, "r") as infile:
                outfile.write(infile.read())
            # Optionally, add a newline separator.
            outfile.write("\n")
            # Write the contents of the original input file.
            with open(abs_input_file, "r") as infile:
                outfile.write(infile.read())
    except Exception as e:
        print(f"Error creating combined checker input: {e}")
        return 0
    


    try:
        with open(combined_checker_input, "r") as f_in:
            result = subprocess.run(["./checker.exe"], stdin=f_in, cwd=intermediate_dir)
            # result = subprocess.run(["./checker", "./intermediate/Edges.txt", combined_checker_input] , cwd=intermediate_dir)
            if result.returncode != 0:
                print("Error running checker.")
                return 0
    except Exception as e:
        print(f"Error running checker: {e}")
        return 0

    # --------------------------------------------------
    # 4. Run Top250.cpp:
    #    It reads from standard input (the original input file) and creates top250.txt in intermediate.
    try:
        with open(abs_input_file, "r") as f_in:
            result = subprocess.run(["./Top250.exe"], stdin=f_in, cwd=intermediate_dir)
            if result.returncode != 0:
                print("Error running Top250.")
                return 0
    except Exception as e:
        print(f"Error running Top250 for {input_file}: {e}")
        return 0
    
    # --------------------------------------------------
    # 5. Compare the first numbers in intermediate/output.txt and intermediate/top250.txt.
    output_txt_path = os.path.join(intermediate_dir, "output.txt")
    output_txt_path_Knapsack = os.path.join(intermediate_dir, "outputKnapsack.txt")
    top250_txt_path = os.path.join(intermediate_dir, "top250.txt")
    
    if not os.path.exists(output_txt_path) or not os.path.exists(top250_txt_path or not os.path.exists(output_txt_path_Knapsack)):
        print("One or both expected output files (output.txt, outputKnapsack.txt, top250.txt) were not created.")
        return 0
    
    try:
        with open(output_txt_path, "r") as f:
            output_contents = f.read().strip()
        with open(top250_txt_path, "r") as f:
            top250_contents = f.read().strip()
        with open(output_txt_path_Knapsack, "r") as f:
            output_contents_Knapsack = f.read().strip()

        # Extract the first token (assumed to be numeric) from each file.
        output_first = float(output_contents.split()[0])
        output_first_Knapsack = float(output_contents_Knapsack.split()[0])
        top250_first = float(top250_contents.split()[0])
    except Exception as e:
        print(f"Error processing output files: {e}")
        return 0
    
    # Determine which file has the larger (or equal) first number.
    current_max = max(output_first, top250_first, output_first_Knapsack)
    
    # Build the final output filename, e.g., "output/output00.txt".
    final_output_filename = os.path.join(final_output_dir, f"output{index:02}.txt")
    
    if output_first >= top250_first and output_first >= output_first_Knapsack:
        print(f"output.txt has the larger (or equal) first number: {output_first} vs {top250_first}")
        with open(final_output_filename, "w") as f:
            f.write(output_contents)
    elif output_first_Knapsack >= top250_first and output_first_Knapsack >= output_first:
        print(f"outputKnapsack.txt has the larger (or equal) first number: {output_first_Knapsack} vs {top250_first}")
        with open(final_output_filename, "w") as f:
            f.write(output_contents_Knapsack)
    else:
        print(f"top250.txt has the larger first number: {top250_first} vs {output_first}")
        with open(final_output_filename, "w") as f:
            f.write(top250_contents)
    
    # Optionally, you could clean up the combined checker file or other intermediate files here.
    return current_max

def main():
    compile_executables()
    total_sum = 0.0
    # Process input files: input00.txt to input19.txt.
    for i in range(20):
        input_filename = os.path.join("input", f"input{i:02}.txt")
        if os.path.exists(input_filename):
            total_sum += run_for_input(input_filename, i)
        else:
            print(f"File {input_filename} does not exist. Skipping.")
    
    print(f"\nSum of all the maximum first numbers is {total_sum}")

if __name__ == '__main__':
    main()
