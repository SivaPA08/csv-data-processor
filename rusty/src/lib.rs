use fasteval::{Evaler, Parser, Slab};
use std::ffi::CStr;
use std::os::raw::c_char;

#[unsafe(no_mangle)]
pub extern "C" fn tokenEval(num: i32, expr_ptr: *const c_char) -> bool {
    if expr_ptr.is_null() {
        return false;
    }

    let c_str = unsafe { CStr::from_ptr(expr_ptr) };
    let expr = match c_str.to_str() {
        Ok(s) => s,
        Err(_) => return false,
    };

    let mut slab = Slab::new();
    let parser = Parser::new();
    let parsed = match parser.parse(expr, &mut slab.ps) {
        Ok(p) => p.from(&slab.ps),
        Err(_) => return false,
    };

    let x = num as f64;
    let mut ns = |name: &str, _| {
        if name == "x" { Some(x) } else { Some(0.0) }
    };
    let eval_result = parsed.eval(&slab, &mut ns);

    match eval_result {
        Ok(v) => v != 0.0,
        Err(_) => false,
    }
}

