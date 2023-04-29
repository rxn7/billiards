-- DEBUG
vim.api.nvim_set_keymap('n', '<f4>', ':te scripts/build.sh<cr>', { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<f5>', ':te scripts/run.sh<cr>', { noremap = true, silent = true })

vim.cmd [[autocmd BufWritePre * lua vim.lsp.buf.format()]]
