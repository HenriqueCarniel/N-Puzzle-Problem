import pandas as pd
import sys

if len(sys.argv) != 2 or sys.argv[1] not in ['-8puzzle', '-15puzzle']:
    print("Usage: script.py -8puzzle or script.py -15puzzle")
    sys.exit(1)

if sys.argv[1] == '-8puzzle':
    algorithms = ['bfs', 'idfs', 'astar', 'idastar', 'gbfs']
    dir_path = 'results/8puzzle_20241026_193931'
else:
    algorithms = ['astar']
    dir_path = 'results/15puzzle_20241026_224329'

print("==========================================")
for algorithm in algorithms:

    file_path = f"{dir_path}/{algorithm}.csv"
    df = pd.read_csv(file_path, header=None, na_values="-", 
        names=["Nodes Expanded", "Solution Length", "Solution Time", "Heuristic Function Mean", "Initial Heuristic Function"])

    avg_nodes_expanded = df["Nodes Expanded"].mean()
    avg_solution_length = df["Solution Length"].mean()
    avg_solution_time = df["Solution Time"].mean()
    avg_heuristic_function = df["Heuristic Function Mean"].mean()
    avg_initial_heuristic_function = df["Initial Heuristic Function"].mean()

    # Exibir as médias arredondadas para o algoritmo atual
    print(f"Results for algorithm {algorithm}:")
    print("------------------------------------------")
    print("Average Nodes Expanded:", avg_nodes_expanded)
    print("Average Solution Length:", avg_solution_length)
    print("Average Solution Time:", avg_solution_time)
    print("Average Heuristic Function:", avg_heuristic_function)
    print("Average Initial Heuristic Function:", avg_initial_heuristic_function)
    print("==========================================")
