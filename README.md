# Repository for Bd simulation and calculation of the amplification factor and acceleration factor

## Evolutionary process
We use a Moran-type model to describe changes in allele frequencies in a finite population of constant size $N$. Each individual's genotype is defined by a single biallelic locus A/a, which controls the individual’s reproductive fitness. An individual with the A allele is assumed to have fitness $1$, while an individual with allele a has assigned fitness $(1 + s)$.

We denote by $p_{fix}$ the probability that a new mutant A fixates in the population of a. We assume that reproduction occurs before death, corresponding to a birth–death (Bd) updating process.

Population structure is represented by a graph $G=(V,E)$, where each node corresponds to an individual and edges encode the local pattern of reproduction and replacement. Specifically, edges determine which individuals can replace one another during reproduction events.

## Amplification factor
The fixation probability of a new mutant in well-mixed (complete) graph is
$$p_{fix, wm}=\frac{1-1/(1+s)}{1/(1+s)^N}$$

In this repository, we define an amplification factor $\alpha$ by expressing the fixation probability on a graph as
$$p_{fix, graph}=\frac{1-1/(1+\alpha s)}{1/(1+\alpha s)^N}$$

An alternative, original definition (also implemented in the script) is
$$p_{fix, graph}=\frac{1-1/(1+s)^{\alpha}}{1/(1+\alpha s)^{\alpha N}}$$

While both parameterizations capture amplification effects and equivalant when $s\ll 1$, the former formulation is numerically more stable and easier to invert using `scipy.solve`.

## Acceleartion factor
For the acceleration factor, we use the definition
$$\lambda=\frac{T_{fix,wm}(s)}{T_{fix,graph}(s)}$$
where $T_{fix}$ denotes the mean fixation time.

A more principled definition would rescale selection in the well-mixed population by the amplification factor
$$\lambda=\frac{T_{fix,wm}(\alpha s)}{T_{fix,graph}(s)}$$
However, this formulation requires substantially longer simulation times and does not yield a measurable improvement in accuracy. We therefore adopt the former definition throughout for computational efficiency here.

## References
[1] Lieberman, Erez, Christoph Hauert, and Martin A. Nowak. "Evolutionary dynamics on graphs." Nature 433.7023 (2005): 312-316.

[2] Kuo, Yang Ping, César Nombela-Arrieta, and Oana Carja. "A theory of evolutionary dynamics on any complex population structure reveals stem cell niche architecture as a spatial suppressor of selection." Nature Communications 15.1 (2024): 4666.

[3] Kuo, Yang Ping, and Oana Carja. "Evolutionary graph theory beyond pairwise interactions: higher-order network motifs shape times to fixation in structured populations." PLOS Computational Biology 20.3 (2024): e1011905.

[4] Kuo, Yang Ping, and Oana Carja. "Evolutionary graph theory beyond single mutation dynamics: on how network-structured populations cross fitness landscapes." Genetics 227.2 (2024): iyae055.