  do { /* BEGIN AUTOGENERATED CODE (n= 7) *****************************/
#   define SORT_STABLE_CE(i,j) u = x[(SORT_IDX_T)i]; v = x[(SORT_IDX_T)j]; c = SORT_BEFORE( v, u ); x[(SORT_IDX_T)i] = c ? v : u; x[(SORT_IDX_T)j] = c ? u : v
    int        c;
    SORT_KEY_T u;
    SORT_KEY_T v;
    switch( n ) {
    case (SORT_IDX_T) 7: SORT_STABLE_CE( 0, 1); SORT_STABLE_CE( 1, 2); SORT_STABLE_CE( 2, 3); SORT_STABLE_CE( 3, 4); SORT_STABLE_CE( 4, 5); SORT_STABLE_CE( 5, 6); /* fall through */
    case (SORT_IDX_T) 6: SORT_STABLE_CE( 0, 1); SORT_STABLE_CE( 1, 2); SORT_STABLE_CE( 2, 3); SORT_STABLE_CE( 3, 4); SORT_STABLE_CE( 4, 5); /* fall through */
    case (SORT_IDX_T) 5: SORT_STABLE_CE( 0, 1); SORT_STABLE_CE( 1, 2); SORT_STABLE_CE( 2, 3); SORT_STABLE_CE( 3, 4); /* fall through */
    case (SORT_IDX_T) 4: SORT_STABLE_CE( 0, 1); SORT_STABLE_CE( 1, 2); SORT_STABLE_CE( 2, 3); /* fall through */
    case (SORT_IDX_T) 3: SORT_STABLE_CE( 0, 1); SORT_STABLE_CE( 1, 2); /* fall through */
    case (SORT_IDX_T) 2: SORT_STABLE_CE( 0, 1); /* fall through */
    case (SORT_IDX_T) 1: /* fall through */
    case (SORT_IDX_T) 0: return x;
    default: break;
    }
#   undef SORT_STABLE_CE
  } while(0); /* END AUTOGENERATED CODE *******************************/