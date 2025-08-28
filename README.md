# Adversarial Robustness and Training

## Overview

This repository explores adversarial robustness in deep learning. The core goals are to:

- Understand why neural networks are vulnerable to adversarial examples
- Implement and compare common adversarial attacks (e.g., FGSM, PGD)
- Train and evaluate models with and without adversarial training
- Analyze the trade-offs between standard accuracy and robust accuracy


## Repository Contents

- `Adversarial Training.ipynb`: The main Jupyter notebook to run experiments, generate adversarial examples, and evaluate robustness.
- `cugwuany - Adversarial Attack Robustness Report.pdf`: A written report summarizing methods, experiments, results, and takeaways.
- `cugwuany - Adversarial Robustness Presentation.pptx.pdf`: Slide deck highlighting key results and insights.


## What You Will Do in the Notebook

While the exact steps may vary by implementation, the notebook typically walks through:

1. Loading a dataset (commonly MNIST/CIFAR)
2. Training a baseline classifier with standard empirical risk minimization (ERM)
3. Generating adversarial examples using:
   - Fast Gradient Sign Method (FGSM)
   - Projected Gradient Descent (PGD)
4. Measuring model accuracy on clean vs. adversarially perturbed inputs
5. Performing adversarial training (e.g., PGD-based training) and re-evaluating robustness
6. Comparing standard accuracy, robust accuracy, and computational cost


## Setup

Recommended environment (Windows PowerShell shown):

```powershell
# Create and activate a virtual environment (Python 3.9–3.11 recommended)
python -m venv .venv
.\.venv\Scripts\Activate.ps1

# Install common dependencies
python -m pip install --upgrade pip
pip install jupyter numpy matplotlib tqdm

# If using PyTorch + CUDA, pick the correct command from https://pytorch.org/get-started/locally/
# Example (CPU-only):
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu
```

If your notebook uses additional libraries (e.g., `scikit-learn`, `torchmetrics`), install them as needed:

```powershell
pip install scikit-learn torchmetrics
```


## Running the Notebook

```powershell
jupyter notebook
```

Then open `Adversarial Training.ipynb` and run the cells sequentially. For reproducibility, set random seeds where prompted and record your environment (Python, PyTorch, CUDA versions).


## Expected Outputs

- Clean vs. adversarial accuracy across different attack strengths (epsilon / steps)
- Visualizations of adversarial examples
- Loss/accuracy curves for standard vs. adversarial training
- Discussion of robustness–accuracy trade-offs

The final results and discussion are captured in the included PDF report and presentation.


## Tips and Notes

- Start with smaller models and subsets of data to validate your pipeline, then scale up.
- For PGD, tune step size and number of steps; ensure the perturbation stays within the epsilon ball via projection.
- Adversarial training increases compute time; consider fewer epochs for debugging.
- Report both standard accuracy and robust accuracy under the same threat model used for training/evaluation.


## Troubleshooting

- "CUDA out of memory": switch to CPU, use smaller batch sizes, or reduce model size.
- Mismatched library versions: verify `torch`, `torchvision`, and Python compatibility.
- Non-determinism: fix seeds and disable certain cuDNN optimizations if exact reproducibility is required.


## Acknowledgments

This project builds on foundational work in adversarial machine learning, including FGSM (Goodfellow et al., 2015) and PGD-based adversarial training (Madry et al., 2018). See the references cited in the accompanying report and presentation for details.


