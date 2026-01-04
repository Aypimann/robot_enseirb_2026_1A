import numpy as np

def im_coordonate_to_unscaled(
        coordonates : list,
        center : tuple,
        pixel_per_radian : float
):
    """
    Convert image coordonates to unscaled vectors

    Parameters
    ----------
    coordonates : tuple of 4 tuples of 2 ints
        corected coordonates of tag corners in image
    center : tuple of 2 floats
        center of image
    pixel_per_radian :
        how much pixel per radian there is at the center of image
    
    Returns
    ----------
    list(np.ndarray)
        A_c, B_c, C_c, D_c vectors (each of shape (3,))
    """
    X_c = [
        np.array((
            (x - center[0]) / pixel_per_radian,
            (y - center[1]) / pixel_per_radian,
            1.
        ))
        for x, y in coordonates
    ]

    return [X_c[i] for i in (0, 1, 3, 2)]




def compute_intersection_direction(Ac, Bc, Cc, Dc):
    """Compute direction vector u = (Ac ^ Bc) ^ (Cc ^ Dc)."""
    n1 = np.cross(Ac, Bc)
    n2 = np.cross(Cc, Dc)
    return np.cross(n1, n2)


def compute_u_XY(Xc, Yc, u):
    """Compute u_XY = alpha * u directly from scalar products."""
    a = np.dot(u, u)
    b = np.dot(u, Yc)
    c = np.dot(Yc, Yc)
    d = np.dot(Xc, u)
    e = np.dot(Xc, Yc)

    # Solve for alpha manually using Cramer's rule
    det = a * c - b * b
    if abs(det) < 1e-8:
        raise ValueError(
            "Singular matrix cannot be inverted in compute_u_XY.")

    alpha = (d * c - b * e) / det
    return -alpha * u


def project_on_tag(Ac, Bc, Cc, Dc, AB_length):
    """
    Project unscaled camera to Aurco corner vectors to their
    acctual location
    
    Parameters
    ----------
    Ac, Bc, Cc, Dc : np.ndarray shape (3,)
        Unscaled vectors A_c, B_c, C_c, D_c.
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

    print("fds ", float(scale_AB), float(scale_CD))
    print(u_AB, u_CD)

    # Step 4: compute PA, PB, PC, PD
    PA = tuple(map(float, scale_AB * Ac))
    PB = tuple(map(float, scale_AB * (Ac + u_AB)))
    PC = tuple(map(float, scale_CD * Cc))
    PD = tuple(map(float, scale_CD * (Cc + u_CD)))

    return PA, PB, PC, PD

