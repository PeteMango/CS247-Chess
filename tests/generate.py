import os

data = """
setup
+ K e4
= black
+ Q e5
done

setup
+ K e4
= black
+ Q e6
done

setup
+ K e4
= black
+ Q e8
done

setup
+ K e4
= black
+ Q a4
done

setup
+ K e4
= black
+ Q h4
done

setup
+ K e4
= black
+ Q h1
done

setup
+ K e4
= black
+ Q a8
done

setup
+ K e4
= black
+ Q b1
done

setup
+ K e4
= black
+ Q h7
done





setup
+ K e4
= black
+ K e5
done

setup
+ K e4
= black
+ K d5
done

setup
+ K e4
= black
+ K f5
done

setup
+ K e4
= black
+ K d4
done

setup
+ K e4
= black
+ K f4
done

setup
+ K e4
= black
+ K d3
done

setup
+ K e4
= black
+ K e3
done

setup
+ K e4
= black
+ K f3
done

setup
+ K e4
= black
+ K a1
done

setup
+ K e4
+ P e5
= black
+ Q e8
done

setup
+ K a8
= black
+ R a1
done





setup
+ K e4
= black
+ R e5
done

setup
+ K e4
= black
+ R e8
done

setup
+ K e4
= black
+ R e3
done

setup
+ K e4
= black
+ R e1
done

setup
+ K e4
= black
+ R d4
done

setup
+ K e4
= black
+ R a4
done

setup
+ K f4
= black
+ R e4
done

setup
+ K e4
= black
+ R h4
done




setup
+ K e4
= black
+ N d2
done

setup
+ K e4
= black
+ N f2
done

setup
+ K e4
= black
+ N c3
done

setup
+ K e4
= black
+ N c5
done

setup
+ K e4
= black
+ N d6
done

setup
+ K e4
= black
+ N f6
done

setup
+ K e4
= black
+ N g5
done

setup
+ K e4
= black
+ N g3
done

setup
+ K e4
= black
+ N g2
done





setup
+ K e4
= black
+ B d3
done

setup
+ K e4
= black
+ B b1
done

setup
+ K e4
= black
+ B f5
done

setup
+ K e4
= black
+ B h7
done

setup
+ K e4
= black
+ B f3
done

setup
+ K e4
= black
+ B h1
done

setup
+ K e4
= black
+ B d5
done

setup
+ K e4
= black
+ B a8
done




setup
+ K e4
= black
+ P f5
done

setup
+ K e4
= black
+ P d5
done

setup
+ K e4
= black
+ P e5
done

setup
+ P d3
= black
+ K e4
done

setup
+ P e3
= black
+ K e4
done

setup
+ P f3
= black
+ K e4
done



setup
+ P d8
done


setup
+ K a8
+ K a7
done

setup
= black
+ K a8
+ K a7
done
"""

# Split the data into blocks
blocks = data.strip().split("\n\n")

# Ensure output directory exists
output_dir = "in/"
os.makedirs(output_dir, exist_ok=True)

# Write each block to a separate file
for i, block in enumerate(blocks, start=1):
    in_name = f"norm{i}.in"
    with open(os.path.join(output_dir, in_name), "w") as f:
        f.write(block.strip() + "\n")

    expect_name = f"norm{i}.in"
    with open(os.path.join("expect/", expect_name), "w") as f:
        f.write("")

print(f'Successfully created {len(blocks)} files in the "{output_dir}" directory.')
