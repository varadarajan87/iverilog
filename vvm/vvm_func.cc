/*
 * Copyright (c) 2000 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#if !defined(WINNT) && !defined(macintosh)
#ident "$Id: vvm_func.cc,v 1.1 2000/03/13 00:02:34 steve Exp $"
#endif

# include  "vvm_func.h"

vvm_bitset_t<1> vvm_unop_and(const vvm_bits_t&r)
{
      vvm_bitset_t<1> res;
      res[0] = r.get_bit(0);

      for (unsigned idx = 1 ;  idx < r.get_width() ;  idx += 1)
	    res[0] = res[0] & r.get_bit(idx);

      return res;
}

vvm_bitset_t<1> vvm_unop_nand(const vvm_bits_t&r)
{
      vvm_bitset_t<1>res = vvm_unop_and(r);
      res[0] = v_not(res[0]);
      return res;
}

vvm_bitset_t<1> vvm_unop_or(const vvm_bits_t&r)
{
      vvm_bitset_t<1> res;
      res[0] = V1;

      for (unsigned idx = 0 ;  idx < r.get_width() ;  idx += 1) {
	    if (r.get_bit(idx) == V1)
		  return res;
      }

      res[0] = V0;
      return res;
}

vvm_bitset_t<1> vvm_unop_nor(const vvm_bits_t&r)
{
      vvm_bitset_t<1>res = vvm_unop_or(r);
      res[0] = v_not(res[0]);
      return res;
}

vvm_bitset_t<1> vvm_unop_xor(const vvm_bits_t&r)
{
      vvm_bitset_t<1> res;
      res[0] = V0;

      for (unsigned idx = 0 ;  idx < r.get_width() ;  idx += 1) {
	    if (r.get_bit(idx) == V1)
		  res[0] = v_not(res[0]);
      }

      return res;
}

vvm_bitset_t<1> vvm_unop_xnor(const vvm_bits_t&r)
{
      vvm_bitset_t<1>res = vvm_unop_xor(r);
      res[0] = v_not(res[0]);
      return res;
}

vvm_bitset_t<1> vvm_binop_eq(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result;
      result[0] = V1;
      const unsigned lwid = l.get_width();
      const unsigned rwid = r.get_width();

      if (lwid <= rwid) {
	    for (unsigned idx = 0 ;  idx < lwid ;  idx += 1) {
		  if ((l.get_bit(idx) == Vx) || (l.get_bit(idx) == Vz)) {
			result[0] = Vx;
			return result;
		  }
		  if ((r.get_bit(idx) == Vx) || (r.get_bit(idx) == Vz)) {
			result[0] = Vx;
			return result;
		  }
		  if (l.get_bit(idx) != r.get_bit(idx)) {
			result[0] = V0;
			return result;
		  }
	    }
	    for (unsigned idx = lwid ;  idx < rwid ;  idx += 1)
		  switch (r.get_bit(idx)) {
		      case V0:
			break;
		      case V1:
			result[0] = V0;
			return result;
		      case Vx:
		      case Vz:
			result[0] = Vx;
			return result;
		  }
		  
	    return result;
      } else {
	    for (unsigned idx = 0 ;  idx < rwid ;  idx += 1) {
		  if ((l.get_bit(idx) == Vx) || (l.get_bit(idx) == Vz)) {
			result[0] = Vx;
			return result;
		  }
		  if ((r.get_bit(idx) == Vx) || (r.get_bit(idx) == Vz)) {
			result[0] = Vx;
			return result;
		  }
		  if (l.get_bit(idx) != r.get_bit(idx)) {
			result[0] = V0;
			return result;
		  }
	    }
	    for (unsigned idx = rwid ;  idx < lwid ;  idx += 1)
		  switch (l.get_bit(idx)) {
		      case V0:
			break;
		      case V1:
			result[0] = V0;
			return result;
		      case Vx:
		      case Vz:
			result[0] = Vx;
			return result;
		  }
		  
	    return result;
      }
}

vvm_bitset_t<1> vvm_binop_ne(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result = vvm_binop_eq(l,r);
      result[0] = v_not(result[0]);
      return result;
}

vvm_bitset_t<1> vvm_binop_eeq(const vvm_bits_t&l,  const vvm_bits_t&r)
{
      vvm_bitset_t<1> result;
      result[0] = V1;
      const unsigned lwid = l.get_width();
      const unsigned rwid = r.get_width();

      if (lwid <= rwid) {
	    for (unsigned idx = 0 ;  idx < lwid ;  idx += 1) {
		  if (l.get_bit(idx) != r.get_bit(idx)) {
			result[0] = V0;
			return result;
		  }
	    }
	    for (unsigned idx = lwid ;  idx < rwid ;  idx += 1)
		  if (r.get_bit(idx) != V0) {
			result[0] = V0;
			return result;
		  }
		  
      } else {
	    for (unsigned idx = 0 ;  idx < rwid ;  idx += 1) {
		  if (l.get_bit(idx) != r.get_bit(idx)) {
			result[0] = V0;
			return result;
		  }
	    }
	    for (unsigned idx = rwid ;  idx < lwid ;  idx += 1)
		  if (l.get_bit(idx) != V0) {
			result[0] = V0;
			return result;
		  }
		  
      }

      return result;
}

vvm_bitset_t<1> vvm_binop_nee(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result = vvm_binop_eeq(l,r);
      result[0] = v_not(result[0]);
      return result;
}

vvm_bitset_t<1> vvm_binop_xeq(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result;
      result[0] = V1;
      const unsigned lwid = l.get_width();
      const unsigned rwid = r.get_width();

      if (lwid <= rwid) {
	    for (unsigned idx = 0 ;  idx < lwid ;  idx += 1) {
		  if ((l.get_bit(idx) == Vz) || (r.get_bit(idx) == Vz))
			continue;
		  if ((l.get_bit(idx) == Vx) || (r.get_bit(idx) == Vx))
			continue;
		  if (l.get_bit(idx) != r.get_bit(idx)) {
			result[0] = V0;
			return result;
		  }
	    }
	    for (unsigned idx = lwid ;  idx < rwid ;  idx += 1) {
		  if ((r.get_bit(idx) == Vx) || (r.get_bit(idx) == Vz))
			continue;
		  if (r.get_bit(idx) != V0) {
			result[0] = V0;
			return result;
		  }
	    }
		  
      } else {
	    for (unsigned idx = 0 ;  idx < rwid ;  idx += 1) {
		  if ((l.get_bit(idx) == Vz) || (r.get_bit(idx) == Vz))
			continue;
		  if ((l.get_bit(idx) == Vx) || (r.get_bit(idx) == Vx))
			continue;
		  if (l.get_bit(idx) != r.get_bit(idx)) {
			result[0] = V0;
			return result;
		  }
	    }
	    for (unsigned idx = rwid ;  idx < lwid ;  idx += 1) {
		  if ((l.get_bit(idx) == Vx) || (l.get_bit(idx) == Vz))
			continue;
		  if (l.get_bit(idx) != V0) {
			result[0] = V0;
			return result;
		  }
	    }
      }

      return result;
}

vvm_bitset_t<1> vvm_binop_zeq(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result;
      result[0] = V1;
      const unsigned lwid = l.get_width();
      const unsigned rwid = r.get_width();

      if (lwid <= rwid) {
	    for (unsigned idx = 0 ;  idx < lwid ;  idx += 1) {
		  if ((l.get_bit(idx) == Vz) || (r.get_bit(idx) == Vz))
			continue;
		  if (l.get_bit(idx) != r.get_bit(idx)) {
			result[0] = V0;
			return result;
		  }
	    }
	    for (unsigned idx = lwid ;  idx < rwid ;  idx += 1) {
		  if (r.get_bit(idx) == Vz)
			continue;
		  if (r.get_bit(idx) != V0) {
			result[0] = V0;
			return result;
		  }
	    }
		  
      } else {
	    for (unsigned idx = 0 ;  idx < rwid ;  idx += 1) {
		  if ((l.get_bit(idx) == Vz) || (r.get_bit(idx) == Vz))
			continue;
		  if (l.get_bit(idx) != r.get_bit(idx)) {
			result[0] = V0;
			return result;
		  }
	    }
	    for (unsigned idx = rwid ;  idx < lwid ;  idx += 1) {
		  if (l.get_bit(idx) == Vz)
			continue;
		  if (l.get_bit(idx) != V0) {
			result[0] = V0;
			return result;
		  }
	    }
      }

      return result;
}

vvm_bitset_t<1> vvm_binop_lt(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result;
      result[0] = V0;
      const unsigned lwid = l.get_width();
      const unsigned rwid = r.get_width();

      const unsigned common = (lwid < rwid)? lwid : rwid;

      for (unsigned idx = 0 ;  idx < common ;  idx += 1)
	    result[0] = less_with_cascade(l.get_bit(idx),
					  r.get_bit(idx),
					  result[0]);

      if (lwid > rwid) {
	    for (unsigned idx = rwid ;  idx < lwid ;  idx += 1)
		  result[0] = less_with_cascade(l.get_bit(idx),
						V0,
						result[0]);
      } else {
	    for (unsigned idx = lwid ;  idx < rwid ;  idx += 1)
		  result[0] = less_with_cascade(V0,
						r.get_bit(idx),
						result[0]);
      }

      return result;
}

vvm_bitset_t<1> vvm_binop_le(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result;
      result[0] = V1;
      const unsigned lwid = l.get_width();
      const unsigned rwid = r.get_width();
      const unsigned common = (lwid < rwid)? lwid : rwid;

      for (unsigned idx = 0 ;  idx < common ;  idx += 1)
	    result[0] = less_with_cascade(l.get_bit(idx),
					  r.get_bit(idx),
					  result[0]);

      if (lwid > rwid) {
	    for (unsigned idx = rwid ;  idx < lwid ;  idx += 1)
		  result[0] = less_with_cascade(l.get_bit(idx),
						V0,
						result[0]);
      } else {
	    for (unsigned idx = lwid ;  idx < rwid ;  idx += 1)
		  result[0] = less_with_cascade(V0,
						r.get_bit(idx),
						result[0]);
      }

      return result;
}

vvm_bitset_t<1> vvm_binop_gt(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result;
      result[0] = V0;

      const unsigned lwid = l.get_width();
      const unsigned rwid = r.get_width();
      const unsigned common = (lwid < rwid)? lwid : rwid;

      for (unsigned idx = 0 ;  idx < common ;  idx += 1)
	    result[0] = greater_with_cascade(l.get_bit(idx),
					     r.get_bit(idx),
					     result[0]);

      if (lwid > rwid) {
	    for (unsigned idx = rwid ;  idx < lwid ;  idx += 1)
		  result[0] = greater_with_cascade(l.get_bit(idx),
						   V0,
						   result[0]);
      } else {
	    for (unsigned idx = lwid ;  idx < rwid ;  idx += 1)
		  result[0] = greater_with_cascade(V0,
						   r.get_bit(idx),
						   result[0]);
      }

      return result;
}

vvm_bitset_t<1> vvm_binop_ge(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> result;
      result[0] = V1;

      const unsigned lwid = l.get_width();
      const unsigned rwid = r.get_width();
      const unsigned common = (lwid < rwid)? lwid : rwid;

      for (unsigned idx = 0 ;  idx < common ;  idx += 1)
	    result[0] = greater_with_cascade(l.get_bit(idx),
					     r.get_bit(idx),
					     result[0]);

      if (lwid > rwid) {
	    for (unsigned idx = rwid ;  idx < lwid ;  idx += 1)
		  result[0] = greater_with_cascade(l.get_bit(idx),
						   V0,
						   result[0]);
      } else {
	    for (unsigned idx = lwid ;  idx < rwid ;  idx += 1)
		  result[0] = greater_with_cascade(V0,
						   r.get_bit(idx),
						   result[0]);
      }

      return result;
}

vvm_bitset_t<1> vvm_binop_land(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> res1 = vvm_unop_or(l);
      vvm_bitset_t<1> res2 = vvm_unop_or(r);
      res1[0] = res1[0] & res2[0];
      return res1;
}

vvm_bitset_t<1> vvm_binop_lor(const vvm_bits_t&l, const vvm_bits_t&r)
{
      vvm_bitset_t<1> res1 = vvm_unop_or(l);
      vvm_bitset_t<1> res2 = vvm_unop_or(r);
      res1[0] = res1[0] | res2[0];
      return res1;
}

vvm_bitset_t<1> vvm_unop_lnot(const vvm_bits_t&r)
{
      vvm_bitset_t<1> res = vvm_unop_or(r);
      return vvm_unop_not(res);
}


/*
 * $Log: vvm_func.cc,v $
 * Revision 1.1  2000/03/13 00:02:34  steve
 *  Remove unneeded templates.
 *
 */

