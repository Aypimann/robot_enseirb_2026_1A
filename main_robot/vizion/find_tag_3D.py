import numpy as np

def compute_intersection_direction(Ac, Bc, Cc, Dc):
    """Compute direction vector u = (Ac × Bc) × (Cc × Dc)."""
    n1 = np.cross(Ac, Bc)
    n2 = np.cross(Cc, Dc)
    u = np.cross(n1, n2)
    return u / np.linalg.norm(u)


def compute_u_XY(Xc, Yc, u):
    """Compute u_XY = alpha * u directly from scalar products."""
    a = np.dot(u, u)
    b = np.dot(u, Yc)
    c = np.dot(Yc, Yc)
    d = np.dot(Xc, u)
    e = np.dot(Xc, Yc)

    # Solve for alpha manually using Cramer's rule
    det = a * c - b * b
    if abs(det) < 1e-12:
        raise ValueError("Singular matrix in compute_u_XY (det ≈ 0).")

    alpha = (d * c - b * e) / det
    return alpha * u


def compute_tag_points(Ac, Bc, Cc, Dc, AB_length):
    """
    Compute PA, PB, PC, PD vectors in 3D from projected tag corners.
    
    Parameters
    ----------
    Ac, Bc, Cc, Dc : np.ndarray shape (3,)
        Projected vectors A_c, B_c, C_c, D_c.
    AB_length : float
        Real-world side length of the tag.
    
    Returns
    -------
    tuple(np.ndarray)
        PA, PB, PC, PD vectors (each of shape (3,))
    """
    # Step 1: compute intersection direction
    u = compute_intersection_direction(Ac, Bc, Cc, Dc)

    # Step 2: compute u_AB and u_CD
    u_AB = compute_u_XY(Ac, Bc, u)
    u_CD = compute_u_XY(Cc, Dc, u)

    # Step 3: scale factors
    scale_AB = AB_length / np.linalg.norm(u_AB)
    scale_CD = AB_length / np.linalg.norm(u_CD)

    # Step 4: compute PA, PB, PC, PD
    PA = scale_AB * Ac
    PB = scale_AB * (Ac + u_AB)
    PC = scale_CD * Cc
    PD = scale_CD * (Cc + u_CD)

    return PA, PB, PC, PD

