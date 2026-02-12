from math import atan

def get_pos(fix_tag, fix_tag_side, fix_tag_pos_C):
    ex = (
        (fix_tag[3][0] - fix_tag[2][0])/fix_tag_side,
        (fix_tag[3][1] - fix_tag[2][1])/fix_tag_side,
        (fix_tag[3][2] - fix_tag[2][2])/fix_tag_side
    )
    ey = (
        (fix_tag[0][0] - fix_tag[2][0])/fix_tag_side,
        (fix_tag[0][1] - fix_tag[2][1])/fix_tag_side,
        (fix_tag[0][2] - fix_tag[2][2])/fix_tag_side
    )
    ez = (
        ex[1] * ey[2] - ex[2] * ey[1],
        ex[2] * ey[0] - ex[0] * ey[2],
        ex[0] * ey[1] - ex[1] * ey[0]
    )
    M = [
        ex[0], ey[0], ez[0],
        ex[1], ey[1], ez[1],
        ex[2], ey[2], ez[2]
    ]
    pos = tuple([
        sum([fix_tag[2][k] * M[i + k] for k in range(3)])
        for i in range(0, 9, 3)
        ]) # TODO faux
    x = M[0]
    y = M[3]
    rot = atan(x/y)
    print("\n\n\n\n\n\n\n", rot, *pos, "\n\n\n\n\n\n\n")
    return rot, *pos

