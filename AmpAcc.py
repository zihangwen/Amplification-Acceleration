# %%
import os
from pathlib import Path
import subprocess
from scipy.optimize import fsolve

# %%
graph_dir=Path("./network")
results_dir = Path("./results_evo")
results_dir.mkdir(parents=True, exist_ok=True)

# %% Graph Generation %% #
# import networkx as nx

# N = 100
# G = nx.star_graph(N-1)  # Create a star graph with 100 nodes (1 center + 99 leaves)

# assert nx.is_connected(G), "Generated graph is not connected!"

# # max component of the graph
# G_max_connected_list = max(nx.connected_components(G), key=len)
# G_max_connected = G.subgraph(G_max_connected_list)

# # Relabel nodes to have consecutive integers starting from 0
# rename_list = [i for i in range(len(G_max_connected.nodes()))]
# mapping = dict(zip(list(G_max_connected.nodes), rename_list))
# G_max_connected = nx.relabel_nodes(G_max_connected, mapping)

# # Save the graph to an edge list file
# graph_path = graph_dir / "star.txt"
# graph_dir.mkdir(parents=True, exist_ok=True)
# nx.write_edgelist(G_max_connected, str(graph_path), data=False)

# # create well-mixed graph (complete graph)
# G_wm = nx.complete_graph(len(G_max_connected.nodes()))
# wm_path = graph_dir / "wm.txt"
# nx.write_edgelist(G_wm, str(wm_path), data=False)

# %% Simulation parameters %% #
graph_name = "detour.txt" # this has to be in edge list format with node labels starting from 0 to N-1
wm_name = "wm.txt"
runs = 100_000
s = 0.1

# running simulation for graph interested:
subprocess.run(
    [
        "src/sim",
        str(graph_dir / graph_name),
        str(results_dir / graph_name),
        str(runs),
        str(s)
    ]
)

# running simulation for graph well-mixed with the same size:
subprocess.run(
    [
        "src/sim",
        str(graph_dir / wm_name),
        str(results_dir / wm_name),
        str(runs),
        str(s)
    ]
)

# %% Reading results %% #
with open(str(results_dir / graph_name)) as f:
    line = f.readline().strip()
N, s, runs, counts, time = line.split("\t")
N = int(N)
s = float(s)
runs = int(runs)
counts = int(counts)
time = float(time)

with open(str(results_dir / wm_name)) as f:
    line = f.readline().strip()
N_wm, s_wm, runs_wm, counts_wm, time_wm = line.split("\t")
N_wm = int(N_wm)
s_wm = float(s_wm)
runs_wm = int(runs_wm)
counts_wm = int(counts_wm)
time_wm = float(time_wm)

assert N == N_wm and s == s_wm and runs == runs_wm, "Graph and well-mixed parameters do not match!"

# %% Calculating amplification ratio and acceleration ratio %% #
p_fix = counts / runs

p_func = lambda alpha, s, N, p_fix: (1 - 1/(1+alpha*s)) / (1 - 1/(1+alpha*s)**N) - p_fix
# p_func = lambda s, alpha, N, p : (1 - (1+s)**(-alpha)) / (1 - (1+s)**(-alpha*N)) - p

alpha = fsolve(p_func, 1, args = (s, N, p_fix)).item()
print(f"Amplification factor (alpha) for graph {graph_name} with s={s}: {alpha}")

acceleration_ratio = time_wm / time
print(f"Acceleration ratio for graph {graph_name} with s={s}: {acceleration_ratio}")

# %%