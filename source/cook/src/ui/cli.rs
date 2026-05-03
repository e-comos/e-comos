use std::env;
use std::process;

pub fn run() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        eprintln!("Usage: cook <command> [options]");
        process::exit(1);
    }

    match args[1].as_str() {
        "search" => search_package(&args[2..]),
        "install" => install_package(&args[2..]),
        "uninstall" => uninstall_package(&args[2..]),
        _ => {
            eprintln!("Unknown command: {}", args[1]);
            process::exit(1);
        }
    }
}

fn search_package(args: &[String]) {
    if args.is_empty() {
        eprintln!("Usage: cook search <package_name>");
        process::exit(1);
    }

    let package_name = &args[0];
    println!("Searching for package: {}", package_name);
    // TODO: Implement search logic
}

fn install_package(args: &[String]) {
    if args.is_empty() {
        eprintln!("Usage: cook install <package_name>");
        process::exit(1);
    }

    let package_name = &args[0];
    println!("Installing package: {}", package_name);
    // TODO: Implement install logic
}

fn uninstall_package(args: &[String]) {
    if args.is_empty() {
        eprintln!("Usage: cook uninstall <package_name>");
        process::exit(1);
    }

    let package_name = &args[0];
    println!("Uninstalling package: {}", package_name);
    // TODO: Implement uninstall logic
}